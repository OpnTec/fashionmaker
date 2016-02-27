# Copyright 1999-2016 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=5

inherit mercurial qmake-utils gnome2-utils fdo-mime

DESCRIPTION="Cloth patternmaking software"
HOMEPAGE="http://www.valentina-project.org/"
SRC_URI=""
EHG_REPO_URI="https://bitbucket.org/dismine/valentina"
EHG_REVISION="develop"

LICENSE="GPL-3"
SLOT="1"
KEYWORDS="~amd64 ~x86"
IUSE="gnome"

# en_IN not supported in Gentoo so not added here
LANGS="ru_RU uk_UA de_DE cs_CZ he_IL fr_FR it_IT nl_NL id_ID es_ES fi_FI en_US en_CA ro_RO zh_CN"

for LANG in ${LANGS}; do
	IUSE="${IUSE} linguas_${LANG}"
done

CDEPEND="
		dev-qt/qtcore:5
		dev-qt/qtgui:5
		dev-qt/qtwidgets:5
		dev-qt/qtxml:5
		dev-qt/qtsvg:5
		dev-qt/linguist:5
		dev-qt/qtxmlpatterns:5
		dev-qt/qtprintsupport:5
		dev-qt/qtnetwork:5
		app-text/poppler"
RDEPEND="${CDEPEND}
DEPEND="${CDEPEND}
		dev-util/ccache"

src_configure() {
	local locales=""

    for LANG in ${LANGS}; do
        if use linguas_${LANG}; then
            locales="${locales} ${LANG}"
        fi
    done
	eqmake5 LOCALES="${locales}" Valentina.pro -r
}

src_install() {
	emake install INSTALL_ROOT="${D}"

	dodoc LICENSE_GPL.txt ChangeLog.txt README.txt

	doman dist/debian/${PN}.1
	doman dist/debian/tape.1

	cp dist/debian/valentina.sharedmimeinfo dist/debian/${PN}.xml || die
	insinto /usr/share/mime/packages
	doins dist/debian/${PN}.xml
}

pkg_postinst() {
	fdo-mime_desktop_database_update

	if use gnome ; then
		gnome2_icon_cache_update
	fi
}
