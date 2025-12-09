OPTS += -O2

UNITY_PATH=../Unity/src
TEST_OPTS  += -I$(UNITY_PATH)

UNITY_SRCS   = $(wildcard $(UNITY_PATH)/*.c)

test_ndigs: test_ndigs.c $(UNITY_SRCS)
	gcc $(OPTS) $(TEST_OPTS) -o $@ test_ndigs.c $(UNITY_SRCS) -lm

