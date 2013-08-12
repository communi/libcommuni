######################################################################
# Communi
######################################################################

TEMPLATE = subdirs
SUBDIRS += auto benchmarks

mac {
    # TODO: install_name_tool?
    check.commands += cd auto &&
    check.commands += DYLD_FRAMEWORK_PATH=$$OUT_PWD/../lib $(MAKE) check &&
    check.commands += cd ../benchmarks &&
    check.commands += DYLD_FRAMEWORK_PATH=$$OUT_PWD/../lib $(MAKE) check
    QMAKE_EXTRA_TARGETS += check
}
