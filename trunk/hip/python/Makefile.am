## $Id$

all: 
	if test @BUILD_PYTHON_MOD@ = yes; then \
		python setup.py build; \
	fi


install: 
	if test @BUILD_PYTHON_MOD@ = yes; then \
		python setup.py install --prefix ${DESTDIR}${prefix}; \
	fi

distclean: clean

clean:
	python setup.py clean --all
