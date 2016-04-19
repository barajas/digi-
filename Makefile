CC = $(CROSS_COMPILE)g++

OBJECTS = simple.o

APP = mainapp

INC= -I/usr/include/opencv -I/usr/include/opencv2 -I/usr/include/opencv2/highgui
LDADD = -L/usr/lib/
LDINC = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy

CFLAGS += -g -O0

all: $(OBJECTS_LDS) $(OBJECTS) $(OBJECTS_ASM) $(SUBDIRS) $(LIBS) $(SOLIBS) $(APP)

$(OBJECTS): %.o: %.c
	$(CC) -c $(CFLAGS) $(INC) $< -o $@
	$(CC) -MM $(CFLAGS) $(INC) $*.c | \
	sed 's,\($(notdir $*)\.o\) *:,$(dir $*.d)\1 $*.d: ,' > $*.d.tmp
	mv $*.d.tmp $*.d

$(APP): $(OBJECTS) $(OBJECTS_ASM) $(LIBRARIES)
	@echo "Linking application $@ for Linux"
	$(CC) \
		-Wl,--start-group \
		$(OBJECTS) \
		$(OBJECTS_ASM) \
		$(LIBRARIES) \
		$(APP_LIBS) \
		-Wl,--end-group \
		-Wl,-rpath=/usr/local/lib \
		$(LDADD) $(LDINC) -o $@

.PHONY: clean
clean::
	rm -f *.o *.d *.a *.map *.bin *.elf \
	$(OBJECTS) $(OBJECTS_LDS) $(OBJECTS_ASM) $(APP) \
		$(patsubst %.o,%.d,$(OBJECTS)) \
		$(patsubst %.o,%.d,$(OBJECTS_ASM))
