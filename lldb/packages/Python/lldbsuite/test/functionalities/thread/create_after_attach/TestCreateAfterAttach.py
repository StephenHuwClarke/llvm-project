"""
Test thread creation after process attach.
"""

from __future__ import print_function


import os
import time
import lldb
from lldbsuite.test.decorators import *
from lldbsuite.test.lldbtest import *
from lldbsuite.test import lldbutil


class CreateAfterAttachTestCase(TestBase):

    mydir = TestBase.compute_mydir(__file__)

    @skipIfFreeBSD  # Hangs.  May be the same as Linux issue llvm.org/pr16229 but
    # not yet investigated.  Revisit once required functionality
    # is implemented for FreeBSD.
    # Occasionally hangs on Windows, may be same as other issues.
    @skipIfWindows
    @skipIfiOSSimulator
    def test_create_after_attach_with_popen(self):
        """Test thread creation after process attach."""
        self.build(dictionary=self.getBuildFlags(use_cpp11=False))
        self.create_after_attach(use_fork=False)

    @skipIfFreeBSD  # Hangs. Revisit once required functionality is implemented
    # for FreeBSD.
    @skipIfRemote
    @skipIfWindows  # Windows doesn't have fork.
    @skipIfiOSSimulator
    def test_create_after_attach_with_fork(self):
        """Test thread creation after process attach."""
        self.build(dictionary=self.getBuildFlags(use_cpp11=False))
        self.create_after_attach(use_fork=True)

    def setUp(self):
        # Call super's setUp().
        TestBase.setUp(self)
        # Find the line numbers for our breakpoints.
        self.break_1 = line_number('main.cpp', '// Set first breakpoint here')
        self.break_2 = line_number('main.cpp', '// Set second breakpoint here')
        self.break_3 = line_number('main.cpp', '// Set third breakpoint here')

    def create_after_attach(self, use_fork):
        """Test thread creation after process attach."""

        exe = self.getBuildArtifact("a.out")

        # Spawn a new process
        if use_fork:
            pid = self.forkSubprocess(exe)
        else:
            popen = self.spawnSubprocess(exe)
            pid = popen.pid
        self.addTearDownHook(self.cleanupSubprocesses)

        # Attach to the spawned process
        self.runCmd("process attach -p " + str(pid))

        target = self.dbg.GetSelectedTarget()

        process = target.GetProcess()
        self.assertTrue(process, PROCESS_IS_VALID)

        # This should create a breakpoint in the main thread.
        lldbutil.run_break_set_by_file_and_line(
            self, "main.cpp", self.break_1, num_expected_locations=1)

        # This should create a breakpoint in the second child thread.
        lldbutil.run_break_set_by_file_and_line(
            self, "main.cpp", self.break_2, num_expected_locations=1)

        # This should create a breakpoint in the first child thread.
        lldbutil.run_break_set_by_file_and_line(
            self, "main.cpp", self.break_3, num_expected_locations=1)

        # Note:  With std::thread, we cannot rely on particular thread numbers.  Using
        # std::thread may cause the program to spin up a thread pool (and it does on
        # Windows), so the thread numbers are non-deterministic.

        # Run to the first breakpoint
        self.runCmd("continue")

        # The stop reason of the thread should be breakpoint.
        self.expect("thread list", STOPPED_DUE_TO_BREAKPOINT,
                    substrs=['stopped',
                             '* thread #',
                             'main',
                             'stop reason = breakpoint'])

        # Change a variable to escape the loop
        self.runCmd("expression main_thread_continue = 1")

        # Run to the second breakpoint
        self.runCmd("continue")

        # The stop reason of the thread should be breakpoint.
        self.expect("thread list", STOPPED_DUE_TO_BREAKPOINT,
                    substrs=['stopped',
                             '* thread #',
                             'thread_2_func',
                             'stop reason = breakpoint'])

        # Change a variable to escape the loop
        self.runCmd("expression child_thread_continue = 1")

        # Run to the third breakpoint
        self.runCmd("continue")

        # The stop reason of the thread should be breakpoint.
        # Thread 3 may or may not have already exited.
        self.expect("thread list", STOPPED_DUE_TO_BREAKPOINT,
                    substrs=['stopped',
                             '* thread #',
                             'thread_1_func',
                             'stop reason = breakpoint'])

        # Run to completion
        self.runCmd("continue")

        # At this point, the inferior process should have exited.
        self.assertTrue(
            process.GetState() == lldb.eStateExited,
            PROCESS_EXITED)
