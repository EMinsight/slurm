AUTOMAKE_OPTIONS = foreign

SUBDIRS = bitstring \
	  hostlist \
	  slurm_protocol_defs \
	  slurm_protocol_pack \
	  slurmdb_defs \
	  slurmdb_pack

AM_CPPFLAGS = -I$(top_srcdir)
LDADD = -ldl -lpthread $(LIB_SLURM)

check_PROGRAMS = \
	$(TESTS)

TESTS = \
	log-test

if HAVE_CHECK
MYCFLAGS  = @CHECK_CFLAGS@ -Wall
MYCFLAGS += -D_ISO99_SOURCE
TESTS += xhash-test \
	 data-test \
	 dns-test \
	 http-test \
	 serializer-test \
	 sluid-test \
	 xstring-test \
	 parse_time-test \
	 job-resources-test \
	 pack-test \
	 reverse_tree-test \
	 xahash-test

xhash_test_CFLAGS = $(MYCFLAGS)
xhash_test_LDADD  = $(LDADD) @CHECK_LIBS@
xahash_test_CFLAGS = $(MYCFLAGS)
xahash_test_LDADD  = $(LDADD) @CHECK_LIBS@
data_test_CFLAGS  = $(MYCFLAGS)
data_test_LDADD   = $(LDADD) @CHECK_LIBS@
dns_test_CFLAGS  = $(MYCFLAGS)
dns_test_LDADD   = $(LDADD) @CHECK_LIBS@
http_test_CFLAGS  = $(MYCFLAGS)
http_test_LDADD   = $(LDADD) @CHECK_LIBS@
serializer_test_CFLAGS  = $(MYCFLAGS)
serializer_test_LDADD   = $(LDADD) @CHECK_LIBS@
sluid_test_CFLAGS = $(MYCFLAGS)
sluid_test_LDADD = $(LDADD) @CHECK_LIBS@
xstring_test_CFLAGS   = $(MYCFLAGS)
xstring_test_LDADD    = $(LDADD) @CHECK_LIBS@
parse_time_test_CFLAGS= $(MYCFLAGS)
parse_time_test_LDADD = $(LDADD) @CHECK_LIBS@
job_resources_test_CFLAGS = $(MYCFLAGS)
job_resources_test_LDADD = $(LDADD) @CHECK_LIBS@
pack_test_CFLAGS = $(MYCFLAGS)
pack_test_LDADD = $(LDADD) @CHECK_LIBS@
reverse_tree_test_CFLAGS = $(MYCFLAGS)
reverse_tree_test_LDADD = $(LDADD) @CHECK_LIBS@

if HAVE_LUA

TESTS += lua-test

AM_CPPFLAGS += $(lua_CFLAGS)

lua_test_CFLAGS = $(MYCFLAGS) \
	-DLUA_TEST_SCRIPT=\"$(top_srcdir)/testsuite/slurm_unit/common/lua-test.lua\"
lua_test_LDADD = \
	$(LDADD) @CHECK_LIBS@ \
	$(top_builddir)/src/lua/libslurm_lua.la \
	$(lua_LIBS)

endif

endif
