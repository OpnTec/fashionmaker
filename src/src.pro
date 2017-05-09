TEMPLATE = subdirs

noTests{ # For enable run qmake with CONFIG+=noTests
	SUBDIRS = \
		libs \
		app

} else {
	SUBDIRS = \
		libs \
		app \
		test

	test.depends = libs
}

app.depends = libs 
