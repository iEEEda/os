INCLUDES        = -I. -I/usr/include

LIBS		= libsocklib.a  \
			-ldl -lpthread -lm

COMPILE_FLAGS   = ${INCLUDES} -c
COMPILE         = gcc ${COMPILE_FLAGS}
LINK            = gcc -o

C_SRCS		= \
		passivesock.c \
		connectsock.c \
		client.c \
		echoserver_thread.c \
		echoserver_simple.c

SOURCE          = ${C_SRCS}

OBJS            = ${SOURCE:.c=.o}

EXEC		= client echoserver echoserver2

.SUFFIXES       :       .o .c .h

all		:	library client echoserver echoserver2

.c.o            :	${SOURCE}
			@echo "    Compiling $< . . .  "
			@${COMPILE} $<

library		:	passivesock.o connectsock.o
			ar rv libsocklib.a passivesock.o connectsock.o

echoserver	:	echoserver_thread.o
			${LINK} $@ echoserver_thread.o ${LIBS}

echoserver2	:	echoserver_simple.o
			${LINK} $@ echoserver_simple.o ${LIBS}

client		:	client.o
			${LINK} $@ client.o ${LIBS}

clean           :
			@echo "    Cleaning ..."
			rm -f tags core *.out *.o *.lis *.a ${EXEC} libsocklib.a
