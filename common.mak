### headers

INCLUDE += -I. -Iwebrtc
INCLUDE += -Iwebrtc/modules/audio_coding/codecs/g711/include
INCLUDE += -Iwebrtc/modules/audio_coding/codecs/g722/include
INCLUDE += -Iwebrtc/common_audio/resampler/include
INCLUDE += -Iwebrtc/common_audio/signal_processing/include


### sources

## base
BASE_ROOT = webrtc/base
SYS_ROOT = webrtc/system_wrappers/source
AUDIO_ROOT = webrtc/common_audio
BASE_SRCS = \
	$(BASE_ROOT)/checks.cc \
	$(SYS_ROOT)/aligned_malloc.cc \
	$(AUDIO_ROOT)/audio_util.cc


## resampler
RESAMPLER_ROOT = webrtc/common_audio/resampler
RESAMPLER_SRCS = \
	$(RESAMPLER_ROOT)/resampler.cc \
	$(RESAMPLER_ROOT)/push_resampler.cc \
	$(RESAMPLER_ROOT)/sinusoidal_linear_chirp_source.cc \
	$(RESAMPLER_ROOT)/push_sinc_resampler.cc \
	$(RESAMPLER_ROOT)/sinc_resampler.cc

ifeq ($(ARCH),x86_64)
RESAMPLER_SRCS += $(RESAMPLER_ROOT)/sinc_resampler_sse.cc
else
ifeq ($(ARCH),i386)
RESAMPLER_SRCS += $(RESAMPLER_ROOT)/sinc_resampler_sse.cc
endif
endif

ifeq ($(ARCH),arm)
RESAMPLER_SRCS += $(RESAMPLER_ROOT)/sinc_resampler_neon.cc
endif


## signal_processing
SIGNAL_ROOT = webrtc/common_audio/signal_processing
SIGNAL_SRCS =  \
	$(SIGNAL_ROOT)/auto_corr_to_refl_coef.c \
	$(SIGNAL_ROOT)/auto_correlation.c \
	$(SIGNAL_ROOT)/complex_bit_reverse.c \
	$(SIGNAL_ROOT)/complex_fft.c \
	$(SIGNAL_ROOT)/copy_set_operations.c \
	$(SIGNAL_ROOT)/cross_correlation.c \
	$(SIGNAL_ROOT)/division_operations.c \
	$(SIGNAL_ROOT)/dot_product_with_scale.c \
	$(SIGNAL_ROOT)/downsample_fast.c \
	$(SIGNAL_ROOT)/energy.c \
	$(SIGNAL_ROOT)/filter_ar.c \
	$(SIGNAL_ROOT)/filter_ar_fast_q12.c \
	$(SIGNAL_ROOT)/filter_ma_fast_q12.c \
	$(SIGNAL_ROOT)/get_hanning_window.c \
	$(SIGNAL_ROOT)/get_scaling_square.c \
	$(SIGNAL_ROOT)/ilbc_specific_functions.c \
	$(SIGNAL_ROOT)/levinson_durbin.c \
	$(SIGNAL_ROOT)/lpc_to_refl_coef.c \
	$(SIGNAL_ROOT)/min_max_operations.c \
	$(SIGNAL_ROOT)/randomization_functions.c \
	$(SIGNAL_ROOT)/real_fft.c \
	$(SIGNAL_ROOT)/refl_coef_to_lpc.c \
	$(SIGNAL_ROOT)/resample.c \
	$(SIGNAL_ROOT)/resample_48khz.c \
	$(SIGNAL_ROOT)/resample_by_2.c \
	$(SIGNAL_ROOT)/resample_by_2_internal.c \
	$(SIGNAL_ROOT)/resample_fractional.c \
	$(SIGNAL_ROOT)/spl_init.c \
	$(SIGNAL_ROOT)/spl_sqrt.c \
	$(SIGNAL_ROOT)/spl_sqrt_floor.c \
	$(SIGNAL_ROOT)/spl_version.c \
	$(SIGNAL_ROOT)/splitting_filter.c \
	$(SIGNAL_ROOT)/sqrt_of_one_minus_x_squared.c \
	$(SIGNAL_ROOT)/vector_scaling_operations.c

ifeq ($(ARCH),arm)
SIGNAL_SRCS += \
	$(SIGNAL_ROOT)/cross_correlation_neon.c \
	$(SIGNAL_ROOT)/downsample_fast_neon.c \
	$(SIGNAL_ROOT)/min_max_operations_neon.c
endif


## audio_coding
ACODEC_ROOT = webrtc/modules/audio_coding/codecs
ACODEC_SRSC = \
	$(ACODEC_ROOT)/g711/g711.c \
	$(ACODEC_ROOT)/g711/g711_interface.c \
	$(ACODEC_ROOT)/g722/g722_decode.c \
	$(ACODEC_ROOT)/g722/g722_interface.c \
	$(ACODEC_ROOT)/g722/g722_encode.c \
	$(ACODEC_ROOT)/opus/opus_interface.c

ISAC_MAIN_ROOT = webrtc/modules/audio_coding/codecs/isac/main
ISAC_MAIN_SRCS = \
	$(ISAC_MAIN_ROOT)/source/arith_routines.c \
	$(ISAC_MAIN_ROOT)/source/arith_routines_hist.c \
	$(ISAC_MAIN_ROOT)/source/arith_routines_logist.c \
	$(ISAC_MAIN_ROOT)/source/bandwidth_estimator.c \
	$(ISAC_MAIN_ROOT)/source/crc.c \
	$(ISAC_MAIN_ROOT)/source/decode.c \
	$(ISAC_MAIN_ROOT)/source/decode_bwe.c \
	$(ISAC_MAIN_ROOT)/source/encode.c \
	$(ISAC_MAIN_ROOT)/source/encode_lpc_swb.c \
	$(ISAC_MAIN_ROOT)/source/entropy_coding.c \
	$(ISAC_MAIN_ROOT)/source/fft.c \
	$(ISAC_MAIN_ROOT)/source/filter_functions.c \
	$(ISAC_MAIN_ROOT)/source/filterbank_tables.c \
	$(ISAC_MAIN_ROOT)/source/filterbanks.c \
	$(ISAC_MAIN_ROOT)/source/intialize.c \
	$(ISAC_MAIN_ROOT)/source/isac.c \
	$(ISAC_MAIN_ROOT)/source/lattice.c \
	$(ISAC_MAIN_ROOT)/source/lpc_analysis.c \
	$(ISAC_MAIN_ROOT)/source/lpc_gain_swb_tables.c \
	$(ISAC_MAIN_ROOT)/source/lpc_shape_swb12_tables.c \
	$(ISAC_MAIN_ROOT)/source/lpc_shape_swb16_tables.c \
	$(ISAC_MAIN_ROOT)/source/lpc_tables.c \
	$(ISAC_MAIN_ROOT)/source/pitch_estimator.c \
	$(ISAC_MAIN_ROOT)/source/pitch_filter.c \
	$(ISAC_MAIN_ROOT)/source/pitch_gain_tables.c \
	$(ISAC_MAIN_ROOT)/source/pitch_lag_tables.c \
	$(ISAC_MAIN_ROOT)/source/spectrum_ar_model_tables.c \
	$(ISAC_MAIN_ROOT)/source/transform.c



### all sources
SRCS = $(BASE_SRCS) $(RESAMPLER_SRCS) \
	$(SIGNAL_SRCS) \
	$(ACODEC_SRSC) \
	$(ISAC_MAIN_SRCS) \
	capi.cc

TMPS = $(SRCS:.cc=.o)
OBJS = $(TMPS:.c=.o)

JNI_SRCS = jni_capi.cc
JNI_TMPS = $(JNI_SRCS:.cc=.o)
JNI_OBJS = $(JNI_TMPS:.c=.o)

TEST_SRCS = tests/test.cc
TEST_TMPS = $(TEST_SRCS:.cc=.o)
TEST_OBJS = $(TEST_TMPS:.c=.o)

TESTJNI_SRCS = tests/test_jni.cc
TESTJNI_TMPS = $(TESTJNI_SRCS:.cc=.o)
TESTJNI_OBJS = $(TESTJNI_TMPS:.c=.o)


### targets

TARGET = capi_acodec
STATIC_TARGET = lib$(TARGET).a
SHARED_TARGET = lib$(TARGET).$(DYEXT)
JNI_SHARED_TARGET = libjni$(TARGET).$(DYEXT)


all: test 

test: $(OBJS) $(TEST_OBJS)
	@$(CXX) -o tests/test $^ $(LDFLAGS)
	@echo "generate test"

testjni: $(OBJS) $(JNI_OBJS) $(TESTJNI_OBJS)
	@$(CXX) -o tests/testjni $^ $(LDFLAGS)
	@echo "generate testjni"


static: $(OBJS)
	@$(AR) $(STATIC_TARGET) $^
	@echo "generate static $(TARGET)"

shared: $(OBJS)
	@$(CXX) $(DFLAGS) -o $(SHARED_TARGET) $^ $(LDFLAGS)
	@echo "generate shared $(TARGET)"

jni: $(OBJS) $(JNI_OBJS)
	@$(CXX) $(DFLAGS) -o $(JNI_SHARED_TARGET) $^ $(LDFLAGS)
	@echo "generate jni-shared $(TARGET)"


.cc.o:
	$(CXX) $(CFLAGS) $(INCLUDE) -c $<  -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $<  -o $@

clean:
	@$(RM) $(OBJS) $(JNI_OBJS)
	@$(RM) $(TEST_OBJS) $(TESTJNI_OBJS)

distclean: clean
	@$(RM) $(STATIC_TARGET)
	@$(RM) $(SHARED_TARGET) $(JNI_SHARED_TARGET)
	@$(RM) tests/test${EXEXT} tests/testjni${EXEXT}


include help.mak
