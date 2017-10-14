The LAME SVN tree contains the following highlevel directories:
 - trunk    -> the msater repository of lame source code, webpages, ...
 - tags     -> a static snapshot of the lame source code for e.g. releases or similar use
 - branches -> feature branches where tests can be made and later merged into trunk

The following info is for developers with read-write access to the LAME SVN, variables
of the form $NAME are used to denote that you need to provide something which fits your
needs.

To checkout all of the lame project:
svn checkout --username=$USERNAME svn+ssh://$USERNAME@svn.code.sf.net/p/lame/svn/trunk $DESTINATION

To only checkout the master branch of the lame source code:
svn checkout --username=$USERNAME svn+ssh://$USERNAME@svn.code.sf.net/p/lame/svn/trunk/lame $DESTINATION

To create a tag, use:
svn copy --username=$USERNAME svn+ssh://$USERNAME@svn.code.sf.net/p/lame/svn/trunk/lame svn+ssh://$USERNAME@svn.code.sf.net/p/lame/svn/tags/$YOUR_TAGNAME

To create a new feature branch, use:
svn copy --username=$USERNAME svn+ssh://$USERNAME@svn.code.sf.net/p/lame/svn/trunk/lame svn+ssh://$USERNAME@svn.code.sf.net/p/lame/svn/branch/$YOUR_BRANCHNAME

To merge back changes into the master branch:
 TO BE FILLED OUT - svn merge ...
Mergin should be done from the root of the branch (.../branch/$YOUR_BRNACHNAME/)
to the root of the master branch (.../trunk/lame/) to have all merge-info consolidated.

