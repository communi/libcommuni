######################################################################
# Communi
######################################################################

defineTest(pkgExists) {
    contains(QT_CONFIG, no-pkg-config):return(false)
    isEmpty(PKG_CONFIG):PKG_CONFIG = pkg-config
    unix:system($$PKG_CONFIG --exists $$1):return(true)
    return(false)
}
