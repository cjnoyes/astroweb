#################################################################
## This Makefile Exported by MinGW Developer Studio
## Copyright (c) 2002-2004 by Parinya Thipchart
#################################################################

ifneq (,$(findstring Release, $(CFG)))
  override CFG = Release
else
  override CFG = Debug
endif

PROJECT = astrowweb
CC = "g++"

ifeq ($(CFG),Debug)
  OBJ_DIR = Debug
  OUTPUT_DIR = Debug
  TARGET = astrowweb.exe
  C_INCLUDE_DIRS = -I"../include" -I"../newplanets" 
  C_PREPROC = 
  CFLAGS = -pipe  -Wall -g3 -O0 -frtti -fexceptions 
  RC_INCLUDE_DIRS = 
  RC_PREPROC = 
  RCFLAGS = 
  LIB_DIRS = 
  LIBS = 
  LDFLAGS = -pipe -g3
endif

ifeq ($(CFG),Release)
  OBJ_DIR = Release
  OUTPUT_DIR = Release
  TARGET = astrowweb.exe
  C_INCLUDE_DIRS = 
  C_PREPROC = 
  CFLAGS = -pipe  -Wall -g0 -O2 -frtti -fexceptions 
  RC_INCLUDE_DIRS = 
  RC_PREPROC = 
  RCFLAGS = 
  LIB_DIRS = 
  LIBS = 
  LDFLAGS = -pipe -s 
endif

ifeq ($(OS),Windows_NT)
  NULL =
else
  NULL = nul
endif

SRC_OBJS = \
  $(OBJ_DIR)/anomaly.o	\
  $(OBJ_DIR)/ascend.o	\
  $(OBJ_DIR)/astduad.o	\
  $(OBJ_DIR)/asthelio.o	\
  $(OBJ_DIR)/astutl.o	\
  $(OBJ_DIR)/charcons.o	\
  $(OBJ_DIR)/cmphouse.o	\
  $(OBJ_DIR)/dataname.o	\
  $(OBJ_DIR)/dohouse.o	\
  $(OBJ_DIR)/eqhouse.o	\
  $(OBJ_DIR)/house.o	\
  $(OBJ_DIR)/hstoramc.o	\
  $(OBJ_DIR)/jtype.o	\
  $(OBJ_DIR)/julian.o	\
  $(OBJ_DIR)/julutil.o	\
  $(OBJ_DIR)/kochhous.o	\
  $(OBJ_DIR)/mctosidt.o	\
  $(OBJ_DIR)/moon.o	\
  $(OBJ_DIR)/morhouse.o	\
  $(OBJ_DIR)/pelement.o	\
  $(OBJ_DIR)/pertjs.o	\
  $(OBJ_DIR)/pertmvm.o	\
  $(OBJ_DIR)/perturnp.o	\
  $(OBJ_DIR)/plachous.o	\
  $(OBJ_DIR)/planets.o	\
  $(OBJ_DIR)/plutocrx.o	\
  $(OBJ_DIR)/reghouse.o	\
  $(OBJ_DIR)/sidereal.o	\
  $(OBJ_DIR)/sun.o	\
  $(OBJ_DIR)/arabpt.o	\
  $(OBJ_DIR)/aspects.o	\
  $(OBJ_DIR)/asteroidcalc.o	\
  $(OBJ_DIR)/astmisc.o	\
  $(OBJ_DIR)/compat.o	\
  $(OBJ_DIR)/compile.o	\
  $(OBJ_DIR)/compile2.o	\
  $(OBJ_DIR)/compos.o	\
  $(OBJ_DIR)/docalc.o	\
  $(OBJ_DIR)/harmonic.o	\
  $(OBJ_DIR)/intrhous.o	\
  $(OBJ_DIR)/loadorbs.o	\
  $(OBJ_DIR)/memmgr.o	\
  $(OBJ_DIR)/midpoint.o	\
  $(OBJ_DIR)/misccalc.o	\
  $(OBJ_DIR)/mktmdt.o	\
  $(OBJ_DIR)/newhouse.o	\
  $(OBJ_DIR)/polarfixup.o	\
  $(OBJ_DIR)/progress.o	\
  $(OBJ_DIR)/realoc.o	\
  $(OBJ_DIR)/solararc.o	\
  $(OBJ_DIR)/transits.o	\
  $(OBJ_DIR)/aa.o	\
  $(OBJ_DIR)/angles.o	\
  $(OBJ_DIR)/arcdot.o	\
  $(OBJ_DIR)/deltat.o	\
  $(OBJ_DIR)/domoon.o	\
  $(OBJ_DIR)/ear404.o	\
  $(OBJ_DIR)/epsiln.o	\
  $(OBJ_DIR)/fk4fk5.o	\
  $(OBJ_DIR)/gplan.o	\
  $(OBJ_DIR)/jup404.o	\
  $(OBJ_DIR)/kepler.o	\
  $(OBJ_DIR)/lonlat.o	\
  $(OBJ_DIR)/mar404.o	\
  $(OBJ_DIR)/mer404.o	\
  $(OBJ_DIR)/messierele.o	\
  $(OBJ_DIR)/mlat404.o	\
  $(OBJ_DIR)/mlr404.o	\
  $(OBJ_DIR)/nep404.o	\
  $(OBJ_DIR)/newplan.o	\
  $(OBJ_DIR)/newplanets.o	\
  $(OBJ_DIR)/nutate.o	\
  $(OBJ_DIR)/plu404.o	\
  $(OBJ_DIR)/precess.o	\
  $(OBJ_DIR)/rplanet.o	\
  $(OBJ_DIR)/rstar.o	\
  $(OBJ_DIR)/sat404.o	\
  $(OBJ_DIR)/sidrlt.o	\
  $(OBJ_DIR)/starele.o	\
  $(OBJ_DIR)/stdafx.o	\
  $(OBJ_DIR)/ura404.o	\
  $(OBJ_DIR)/utilfnc.o	\
  $(OBJ_DIR)/vearth.o	\
  $(OBJ_DIR)/ven404.o	\
  $(OBJ_DIR)/zatan2.o	\
  $(OBJ_DIR)/errors.o	\
  $(OBJ_DIR)/files.o	\
  $(OBJ_DIR)/makename.o	\
  $(OBJ_DIR)/strclip.o	\
  $(OBJ_DIR)/newsun.o	\
  $(OBJ_DIR)/setpath.o	\
  $(OBJ_DIR)/ChartWriter.o	\
  $(OBJ_DIR)/DataReader.o	\
  $(OBJ_DIR)/OptionsReader.o	\
  $(OBJ_DIR)/globals.o	\
  $(OBJ_DIR)/main.o

define build_target
@echo Linking...
@$(CC) -o "$(OUTPUT_DIR)/$(TARGET)" $(SRC_OBJS) $(LIB_DIRS) $(LIBS) $(LDFLAGS)
endef

define compile_source
@echo Compiling $<
@$(CC) $(CFLAGS) $(C_PREPROC) $(C_INCLUDE_DIRS) -c "$<" -o "$@"
endef

.PHONY: print_header directories

$(TARGET): print_header directories $(SRC_OBJS)
	$(build_target)

.PHONY: clean cleanall

cleanall:
	@echo Deleting intermediate files for 'astrowweb - $(CFG)'
	-@del $(OBJ_DIR)/*.o
	-@del "$(OUTPUT_DIR)/$(TARGET)"
	-@rmdir "$(OUTPUT_DIR)"

clean:
	@echo Deleting intermediate files for 'astrowweb - $(CFG)'
	-@del $(OBJ_DIR)/*.o

print_header:
	@echo ----------Configuration: astrowweb - $(CFG)----------

directories:
	-@if not exist "$(OUTPUT_DIR)/$(NULL)" mkdir "$(OUTPUT_DIR)"
	-@if not exist "$(OBJ_DIR)/$(NULL)" mkdir "$(OBJ_DIR)"

$(OBJ_DIR)/anomaly.o: ../engdll/anomaly.cpp

	$(compile_source)

$(OBJ_DIR)/ascend.o: ../engdll/ascend.cpp

	$(compile_source)

$(OBJ_DIR)/astduad.o: ../engdll/astduad.cpp

	$(compile_source)

$(OBJ_DIR)/asthelio.o: ../engdll/asthelio.cpp

	$(compile_source)

$(OBJ_DIR)/astutl.o: ../engdll/astutl.cpp

	$(compile_source)

$(OBJ_DIR)/charcons.o: ../engdll/charcons.cpp

	$(compile_source)

$(OBJ_DIR)/cmphouse.o: ../engdll/cmphouse.cpp

	$(compile_source)

$(OBJ_DIR)/dataname.o: ../engdll/dataname.cpp

	$(compile_source)

$(OBJ_DIR)/dohouse.o: ../engdll/dohouse.cpp

	$(compile_source)

$(OBJ_DIR)/eqhouse.o: ../engdll/eqhouse.cpp

	$(compile_source)

$(OBJ_DIR)/house.o: ../engdll/house.cpp

	$(compile_source)

$(OBJ_DIR)/hstoramc.o: ../engdll/hstoramc.cpp

	$(compile_source)

$(OBJ_DIR)/jtype.o: ../engdll/jtype.cpp

	$(compile_source)

$(OBJ_DIR)/julian.o: ../engdll/julian.cpp

	$(compile_source)

$(OBJ_DIR)/julutil.o: ../engdll/julutil.cpp

	$(compile_source)

$(OBJ_DIR)/kochhous.o: ../engdll/kochhous.cpp

	$(compile_source)

$(OBJ_DIR)/mctosidt.o: ../engdll/mctosidt.cpp

	$(compile_source)

$(OBJ_DIR)/moon.o: ../engdll/moon.cpp

	$(compile_source)

$(OBJ_DIR)/morhouse.o: ../engdll/morhouse.cpp

	$(compile_source)

$(OBJ_DIR)/pelement.o: ../engdll/pelement.cpp

	$(compile_source)

$(OBJ_DIR)/pertjs.o: ../engdll/pertjs.cpp

	$(compile_source)

$(OBJ_DIR)/pertmvm.o: ../engdll/pertmvm.cpp

	$(compile_source)

$(OBJ_DIR)/perturnp.o: ../engdll/perturnp.cpp

	$(compile_source)

$(OBJ_DIR)/plachous.o: ../engdll/plachous.cpp

	$(compile_source)

$(OBJ_DIR)/planets.o: ../engdll/planets.cpp

	$(compile_source)

$(OBJ_DIR)/plutocrx.o: ../engdll/plutocrx.cpp

	$(compile_source)

$(OBJ_DIR)/reghouse.o: ../engdll/reghouse.cpp

	$(compile_source)

$(OBJ_DIR)/sidereal.o: ../engdll/sidereal.cpp

	$(compile_source)

$(OBJ_DIR)/sun.o: ../engdll/sun.cpp

	$(compile_source)

$(OBJ_DIR)/arabpt.o: ../engine/arabpt.cpp

	$(compile_source)

$(OBJ_DIR)/aspects.o: ../engine/aspects.cpp

	$(compile_source)

$(OBJ_DIR)/asteroidcalc.o: ../engine/asteroidcalc.cpp

	$(compile_source)

$(OBJ_DIR)/astmisc.o: ../engine/astmisc.cpp

	$(compile_source)

$(OBJ_DIR)/compat.o: ../engine/compat.cpp

	$(compile_source)

$(OBJ_DIR)/compile.o: ../engine/compile.cpp

	$(compile_source)

$(OBJ_DIR)/compile2.o: ../engine/compile2.cpp

	$(compile_source)

$(OBJ_DIR)/compos.o: ../engine/compos.cpp

	$(compile_source)

$(OBJ_DIR)/docalc.o: ../engine/docalc.cpp

	$(compile_source)

$(OBJ_DIR)/harmonic.o: ../engine/harmonic.cpp

	$(compile_source)

$(OBJ_DIR)/intrhous.o: ../engine/intrhous.cpp

	$(compile_source)

$(OBJ_DIR)/loadorbs.o: ../engine/loadorbs.cpp

	$(compile_source)

$(OBJ_DIR)/memmgr.o: ../engine/memmgr.cpp

	$(compile_source)

$(OBJ_DIR)/midpoint.o: ../engine/midpoint.cpp

	$(compile_source)

$(OBJ_DIR)/misccalc.o: ../engine/misccalc.cpp

	$(compile_source)

$(OBJ_DIR)/mktmdt.o: ../engine/mktmdt.cpp

	$(compile_source)

$(OBJ_DIR)/newhouse.o: ../engine/newhouse.cpp

	$(compile_source)

$(OBJ_DIR)/polarfixup.o: ../engine/polarfixup.cpp

	$(compile_source)

$(OBJ_DIR)/progress.o: ../engine/progress.cpp

	$(compile_source)

$(OBJ_DIR)/realoc.o: ../engine/realoc.cpp

	$(compile_source)

$(OBJ_DIR)/solararc.o: ../engine/solararc.cpp

	$(compile_source)

$(OBJ_DIR)/transits.o: ../engine/transits.cpp

	$(compile_source)

$(OBJ_DIR)/aa.o: ../newplanets/aa.cpp	\
../newplanets/stdafx.h	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/angles.o: ../newplanets/angles.cpp	\
../newplanets/stdafx.h	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/arcdot.o: ../newplanets/arcdot.cpp	\
../newplanets/stdafx.h
	$(compile_source)

$(OBJ_DIR)/deltat.o: ../newplanets/deltat.cpp	\
../newplanets/stdafx.h	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/domoon.o: ../newplanets/domoon.cpp	\
../newplanets/stdafx.h	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/ear404.o: ../newplanets/ear404.cpp	\
../newplanets/stdafx.h	\
../newplanets/plantbl.h
	$(compile_source)

$(OBJ_DIR)/epsiln.o: ../newplanets/epsiln.cpp	\
../newplanets/stdafx.h
	$(compile_source)

$(OBJ_DIR)/fk4fk5.o: ../newplanets/fk4fk5.cpp	\
../newplanets/stdafx.h	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/gplan.o: ../newplanets/gplan.cpp	\
../newplanets/stdafx.h	\
../newplanets/plantbl.h
	$(compile_source)

$(OBJ_DIR)/jup404.o: ../newplanets/jup404.cpp	\
../newplanets/stdafx.h	\
../newplanets/plantbl.h
	$(compile_source)

$(OBJ_DIR)/kepler.o: ../newplanets/kepler.cpp	\
../newplanets/stdafx.h	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/lonlat.o: ../newplanets/lonlat.cpp	\
../newplanets/stdafx.h	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/mar404.o: ../newplanets/mar404.cpp	\
../newplanets/stdafx.h	\
../newplanets/plantbl.h
	$(compile_source)

$(OBJ_DIR)/mer404.o: ../newplanets/mer404.cpp	\
../newplanets/stdafx.h	\
../newplanets/plantbl.h
	$(compile_source)

$(OBJ_DIR)/messierele.o: ../newplanets/messierele.cpp	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/mlat404.o: ../newplanets/mlat404.cpp	\
../newplanets/stdafx.h	\
../newplanets/plantbl.h
	$(compile_source)

$(OBJ_DIR)/mlr404.o: ../newplanets/mlr404.cpp	\
../newplanets/stdafx.h	\
../newplanets/plantbl.h
	$(compile_source)

$(OBJ_DIR)/nep404.o: ../newplanets/nep404.cpp	\
../newplanets/stdafx.h	\
../newplanets/plantbl.h
	$(compile_source)

$(OBJ_DIR)/newplan.o: ../newplanets/newplan.cpp	\
../newplanets/stdafx.h	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/newplanets.o: ../newplanets/newplanets.cpp	\
../newplanets/stdafx.h	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/nutate.o: ../newplanets/nutate.cpp	\
../newplanets/stdafx.h	\
../newplanets/plantbl.h
	$(compile_source)

$(OBJ_DIR)/plu404.o: ../newplanets/plu404.cpp	\
../newplanets/stdafx.h	\
../newplanets/plantbl.h
	$(compile_source)

$(OBJ_DIR)/precess.o: ../newplanets/precess.cpp	\
../newplanets/stdafx.h
	$(compile_source)

$(OBJ_DIR)/rplanet.o: ../newplanets/rplanet.cpp	\
../newplanets/stdafx.h	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/rstar.o: ../newplanets/rstar.cpp	\
../newplanets/stdafx.h	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/sat404.o: ../newplanets/sat404.cpp	\
../newplanets/stdafx.h	\
../newplanets/plantbl.h
	$(compile_source)

$(OBJ_DIR)/sidrlt.o: ../newplanets/sidrlt.cpp	\
../newplanets/stdafx.h
	$(compile_source)

$(OBJ_DIR)/starele.o: ../newplanets/starele.cpp	\
../newplanets/stdafx.h	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/stdafx.o: ../newplanets/stdafx.cpp	\
../newplanets/stdafx.h
	$(compile_source)

$(OBJ_DIR)/ura404.o: ../newplanets/ura404.cpp	\
../newplanets/stdafx.h	\
../newplanets/plantbl.h
	$(compile_source)

$(OBJ_DIR)/utilfnc.o: ../newplanets/utilfnc.cpp	\
../newplanets/stdafx.h	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/vearth.o: ../newplanets/vearth.cpp	\
../newplanets/stdafx.h	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/ven404.o: ../newplanets/ven404.cpp	\
../newplanets/stdafx.h	\
../newplanets/plantbl.h
	$(compile_source)

$(OBJ_DIR)/zatan2.o: ../newplanets/zatan2.cpp	\
../newplanets/stdafx.h
	$(compile_source)

$(OBJ_DIR)/errors.o: ../uiutil/errors.cpp

	$(compile_source)

$(OBJ_DIR)/files.o: ../uiutil/files.cpp

	$(compile_source)

$(OBJ_DIR)/makename.o: ../uiutil/makename.cpp

	$(compile_source)

$(OBJ_DIR)/strclip.o: ../uiutil/strclip.cpp

	$(compile_source)

$(OBJ_DIR)/newsun.o: ../newplanets/newsun.cpp	\
../newplanets/stdafx.h	\
../newplanets/kep.h	\
../newplanets/plantbl.h	\
../newplanets/protos.h
	$(compile_source)

$(OBJ_DIR)/setpath.o: ../uiutil/setpath.cpp

	$(compile_source)

$(OBJ_DIR)/ChartWriter.o: ../io/ChartWriter.cpp

	$(compile_source)

$(OBJ_DIR)/DataReader.o: ../io/DataReader.cpp

	$(compile_source)

$(OBJ_DIR)/OptionsReader.o: ../io/OptionsReader.cpp

	$(compile_source)

$(OBJ_DIR)/globals.o: ../uiutil/globals.cpp

	$(compile_source)

$(OBJ_DIR)/main.o: ../uiutil/main.cpp

	$(compile_source)

