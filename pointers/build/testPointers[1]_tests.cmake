add_test( test.test [==[/home/juliengauthier410/l3/s6/programmation_Multi-Paradigme/projet 1/pointers/build/testPointers]==] [==[--gtest_filter=test.test]==] --gtest_also_run_disabled_tests)
set_tests_properties( test.test PROPERTIES WORKING_DIRECTORY [==[/home/juliengauthier410/l3/s6/programmation_Multi-Paradigme/projet 1/pointers/build]==] SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( testPointers_TESTS test.test)
