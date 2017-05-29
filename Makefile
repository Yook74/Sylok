################################################
# Two changes are needed.
# (1) Add your player's .o file to CONTESTOBJECTS
# (2) Add your player's dependencies right after DumbPlyaerV2's.
################################################

CXXFLAGS = -g -Wall -O2
CXX = g++


################################################
# Change 1:
# Add your player on the line after GamblerPlayer 
################################################
CONTESTOBJECTS = AIContest.o BoardV3.o Message.o PlayerV2.o conio.o contest.o \
	DumbPlayerV2.o CleanPlayerV2.o GamblerPlayerV2.o Sylok.o LearningGambler.o

TESTEROBJECTS = AIContest.o BoardV3.o Message.o PlayerV2.o conio.o tester.o \
	DumbPlayerV2.o CleanPlayerV2.o GamblerPlayerV2.o SemiSmartPlayerV2.o

instructions:
	@echo "Make options: contest, testAI, clean, tar"

contest: $(CONTESTOBJECTS)
	g++ -o contest $(CONTESTOBJECTS)
	@echo "Contest binary is in 'contest'. Run as './contest'"

testAI: $(TESTEROBJECTS) 
	g++ -o testAI $(TESTEROBJECTS)
	@echo "Tester is in 'testAI'. Run as './testAI'"

clean:
	rm -f contest $(CONTESTOBJECTS) $(TESTEROBJECTS)

tar:
	g++ -c -Wall -O2 CleanPlayerV2.cpp
	g++ -c -Wall -O2 SemiSmartPlayerV2.cpp
	g++ -c -Wall -O2 GamblerPlayerV2.cpp
	g++ -c -Wall -O2 BoardV3.cpp
	tar -cvf binaries.tar CleanPlayerV2.o GamblerPlayerV2.o BoardV3.o SemiSmartPlayerV2.o

contest.o: contest.cpp
contest.cpp: defines.h Message.cpp

tester.o: tester.cpp
tester.cpp: defines.h Message.cpp

Message.o: Message.cpp
Message.cpp: Message.h defines.h

conio.o: conio.cpp
conio.cpp: conio.h

AIContest.o: AIContest.cpp
AIContest.cpp: AIContest.h Message.h defines.h

# Players here
Sylok.o: Sylok.cpp Message.cpp
Sylok.cpp: Sylok.h defines.h PlayerV2.h conio.cpp


################################################
# Change 2:
# Add your player dependency information below
################################################


# CleanPlayerV2.o and other provided binaries are only available as a linkable Linux binary, not as source code.
CleanPlayerV2.o: 
	tar -xvf binaries.tar CleanPlayerV2.o
GamblerPlayerV2.o:
	tar -xvf binaries.tar GamblerPlayerV2.o
SemiSmartPlayerV2.o: 
	tar -xvf binaries.tar SemiSmartPlayerV2.o
BoardV3.o:
	tar -xvf binaries.tar BoardV3.o
LearningGambler.o:
	tar -xvf binaries.tar LearningGambler.o

