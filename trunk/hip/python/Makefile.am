## $Id$

all: 
	if test @BUILD_PYTHON_MOD@ = yes; then \
		python2.1 setup.py build; \
	fi


install: 
	if test @BUILD_PYTHON_MOD@ = yes; then \
		python2.1 setup.py install --prefix ${DESTDIR}${prefix}; \
	fi

distclean: clean

clean:
	python2.1 setup.py clean --all
