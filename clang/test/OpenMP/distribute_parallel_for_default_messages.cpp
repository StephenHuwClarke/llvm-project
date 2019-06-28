// RUN: %clang_cc1 -verify -fopenmp -ferror-limit 100 -o - %s

// RUN: %clang_cc1 -verify -fopenmp-simd -ferror-limit 100 -o - %s

void foo();

template <class T, int N>
T tmain(T argc) {
  int i;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default // expected-error {{expected '(' after 'default'}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default( // expected-error {{expected 'none' or 'shared' in OpenMP clause 'default'}} expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default() // expected-error {{expected 'none' or 'shared' in OpenMP clause 'default'}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default(none // expected-error {{expected ')'}} expected-note {{to match this '('}} expected-note 2 {{explicit data sharing attribute requested here}}
  for (i = 0; i < argc; ++i) // expected-error 2 {{variable 'argc' must have explicitly specified data sharing attributes}}
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default(shared), default(shared) // expected-error {{directive '#pragma omp distribute parallel for' cannot contain more than one 'default' clause}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default(x) // expected-error {{expected 'none' or 'shared' in OpenMP clause 'default'}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default(none) // expected-note 2 {{explicit data sharing attribute requested here}}
  for (i = 0; i < argc; ++i)  // expected-error 2 {{variable 'argc' must have explicitly specified data sharing attributes}}
    foo();

#pragma omp parallel default(none) // expected-note 2 {{explicit data sharing attribute requested here}}
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default(shared)
  for (i = 0; i < argc; ++i) // expected-error 2 {{variable 'argc' must have explicitly specified data sharing attributes}}
    foo();

  return T();
}

int main(int argc, char **argv) {
  int i;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default // expected-error {{expected '(' after 'default'}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default( // expected-error {{expected 'none' or 'shared' in OpenMP clause 'default'}} expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default() // expected-error {{expected 'none' or 'shared' in OpenMP clause 'default'}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default(none // expected-error {{expected ')'}} expected-note {{to match this '('}} expected-note {{explicit data sharing attribute requested here}}
  for (i = 0; i < argc; ++i) // expected-error {{variable 'argc' must have explicitly specified data sharing attributes}}
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default(shared), default(shared) // expected-error {{directive '#pragma omp distribute parallel for' cannot contain more than one 'default' clause}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default(x) // expected-error {{expected 'none' or 'shared' in OpenMP clause 'default'}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default(none) // expected-note {{explicit data sharing attribute requested here}}
  for (i = 0; i < argc; ++i)  // expected-error {{variable 'argc' must have explicitly specified data sharing attributes}}
    foo();

#pragma omp parallel default(none) // expected-note {{explicit data sharing attribute requested here}}
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for default(shared)
  for (i = 0; i < argc; ++i) // expected-error {{variable 'argc' must have explicitly specified data sharing attributes}}
    foo();

  return (tmain<int, 5>(argc) + tmain<char, 1>(argv[0][0])); // expected-note {{in instantiation of function template specialization 'tmain<int, 5>' requested here}} expected-note {{in instantiation of function template specialization 'tmain<char, 1>' requested here}}
}
