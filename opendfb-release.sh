make clean;
find -name *.libs | xargs rm -rf;
find -name *.lo | xargs rm -rf;
find -name *deps | xargs rm -rf;
find -name Makefile | xargs rm -rf;
find -name Makefile.in | xargs rm -rf;
find -name "*.pc" | xargs rm -rf;
rm autom4te.cache/ -rf;
rm stamp-h1 directfb.pc config.status config.log config.h -rf;
