######################################################################
# LibIrcClient-Qt
######################################################################

contains(CONFIG, coverage) {
    OBJECTS_DIR =
    MOC_DIR =

    QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
    QMAKE_LIBS += -lgcov
    QMAKE_CLEAN += *.gcno

    zerocounters.commands = @lcov --directory \$(OBJECTS_DIR) --zerocounters
    QMAKE_EXTRA_UNIX_TARGETS *= zerocounters

    capture.commands = @mkdir -p coverage
    capture.commands += && lcov --directory \$(OBJECTS_DIR) --capture --output-file coverage/coverage.info
    capture.filters = \"/usr/*\" \"moc_*.cpp\"
    capture.commands += && lcov --remove coverage/coverage.info $$capture.filters --output-file coverage/coverage.info
    QMAKE_EXTRA_UNIX_TARGETS *= capture

    genhtml.commands = @genhtml --output-directory coverage coverage/coverage.info
    genhtml.commands += && xdg-open coverage/index.html
    QMAKE_EXTRA_UNIX_TARGETS *= genhtml

    runtests.commands += $(QMAKE) tests/tests.pro && $(MAKE) -C tests test
    QMAKE_EXTRA_UNIX_TARGETS += runtests

    coverage.depends += first zerocounters runtests capture genhtml
    QMAKE_EXTRA_UNIX_TARGETS += coverage
}
