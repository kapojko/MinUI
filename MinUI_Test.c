#ifdef TESTING

#include <stdio.h>
#include "MinUnit.h"
#include "UIView.h"
#include "Utf8.h"

int tests_run = 0;

static const char *RunAllTests(void) {
    mu_run_test(UIView_UnitTest);
    mu_run_test(Utf8_UnitTest);
    return 0;
}

int main(void) {
    const char *result = RunAllTests();
    if (result) {
        printf("TEST FAILED: %s\n", result);
        return 1;
    } else {
        printf("ALL TESTS PASSED (%d)\n", tests_run);
        return 0;
    }
}

#endif
