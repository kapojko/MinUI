#ifdef TESTING

#include <stdio.h>
#include "UIView.h"

int tests_run = 0;

static const char *RunAllTests(void) {
    mu_run_test(UIView_UnitTest);
    return 0;
}

int main(void) {
    const char *result = RunAllTests();
    if (result) {
        printf("TEST FAILED: %s\n", result);
    } else {
        printf("ALL TESTS PASSED (%d)\n", tests_run);
    }
}

#endif
