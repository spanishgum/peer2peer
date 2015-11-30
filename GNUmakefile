#################################################################
#                                                               #
#         Name : Adam Stallard                                  #
#        Class : COP5570                                        #
#   Assignment : a Gnutella like peer to peer system            #
#      Compile : "make fresh"                                   #
#      Execute : "bin/g04"                                      #
#                                                               #
#################################################################

#####################################################################

define HEADER

   ----------------------------------------------
   |                                            |
   |                   UShare                   |
   |            a Peer to Peer System           |
   |               by Adam Stallard             |
   |                                            |
   ----------------------------------------------

   . . . building components . . .

endef

define FOOTER

   . . . components finished . . .

endef

export HEADER FOOTER

#####################################################################

SHELL = /bin/sh
CCC = g++ -std=c++11
CCFLAGS = -g -Wall -pedantic -lpthread
# CCFLAGS = -Wall -pedantic -O3
SRCS := $(wildcard src/*.cpp)
HDRS := $(wildcard hdr/*.h)
OBJS := $(patsubst src/%.cpp, obj/%.o, $(SRCS))
PRGS := bin/g04

#####################################################################

.PHONY : fresh clean cls hdr ftr
.SUFFIXES :

#####################################################################

$(PRGS) : $(OBJS)
	$(CCC) $(CCFLAGS) $(OBJS) -o $@

# obj/%.o : src/%.cpp src/%.h
obj/%.o : src/%.cpp
	$(CCC) $(CCFLAGS) -c $< -o $@

#####################################################################

fresh : clean cls hdr $(PRGS) ftr

clean :
	-@rm $(PRGS) $(OBJS) 2>/dev/null || true

cls :
	@clear

hdr :
	@$(info $(HEADER))

ftr :
	@$(info $(FOOTER))


#####################################################################
