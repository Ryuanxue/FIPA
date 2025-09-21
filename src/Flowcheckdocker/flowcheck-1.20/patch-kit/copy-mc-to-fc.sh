mkdir exp-flowcheck
perl -pe 's/mc_/fc_/g; s/mcinclude/fcinclude/g; s/MEMCHECK/FLOWCHECK/g; s/memcheck-(x86|amd64|ppc32|ppc64)-(linux|aix5)/exp-flowcheck-$1-$2/g; s/memcheck_(x86|amd64|ppc32|ppc64)_(linux|aix5)/exp_flowcheck_$1_$2/g; s/memcheck/flowcheck/g' memcheck/Makefile.am >exp-flowcheck/Makefile.am
perl -pe 's/mc_/fc_/g; s/MC_/FC_/g; s/mcinclude/fcinclude/g; s/MEMCHECK/FLOWCHECK/g; s/MemCheck/FlowCheck/g; s/memcheck/flowcheck/g' memcheck/mc_include.h >exp-flowcheck/fc_include.h
perl -pe 's/mc_/fc_/g; s/MC_/FC_/g; s/mcinclude/fcinclude/g; s/MEMCHECK/FLOWCHECK/g; s/MemCheck/FlowCheck/g; s/memcheck/flowcheck/g' memcheck/mc_leakcheck.c >exp-flowcheck/fc_leakcheck.c
perl -pe 's/mc_/fc_/g; s/MC_/FC_/g; s/mcinclude/fcinclude/g; s/MEMCHECK/FLOWCHECK/g; s/MemCheck/FlowCheck/g; s/memcheck/flowcheck/g; s/\047M\047,/\047F\047,/g' memcheck/mc_main.c >exp-flowcheck/fc_main.c
perl -pe 's/mc_/fc_/g; s/MC_/FC_/g; s/mcinclude/fcinclude/g; s/MEMCHECK/FLOWCHECK/g; s/MemCheck/FlowCheck/g; s/memcheck/flowcheck/g; s/\047M\047,/\047F\047,/g' memcheck/mc_errors.c >exp-flowcheck/fc_errors.c
perl -pe 's/mc_/fc_/g; s/MC_/FC_/g; s/mcinclude/fcinclude/g; s/MEMCHECK/FLOWCHECK/g; s/MemCheck/FlowCheck/g; s/memcheck/flowcheck/g; s/\047M\047,/\047F\047,/g' memcheck/mc_machine.c >exp-flowcheck/fc_machine.c
perl -pe 's/mc_/fc_/g; s/MC_/FC_/g; s/mcinclude/fcinclude/g; s/MEMCHECK/FLOWCHECK/g; s/MemCheck/FlowCheck/g; s/memcheck/flowcheck/g' memcheck/mc_malloc_wrappers.c >exp-flowcheck/fc_malloc_wrappers.c
perl -pe 's/mc_/fc_/g; s/MC_/FC_/g; s/mcinclude/fcinclude/g; s/MEMCHECK/FLOWCHECK/g; s/MemCheck/FlowCheck/g; s/memcheck/flowcheck/g' memcheck/mc_replace_strmem.c >exp-flowcheck/fc_replace_strmem.c
perl -pe 's/mc_/fc_/g; s/MC_/FC_/g; s/mcinclude/fcinclude/g; s/MEMCHECK/FLOWCHECK/g; s/MemCheck/FlowCheck/g; s/memcheck/flowcheck/g; s/MCE/FCE/g; s/mce/fce/g' memcheck/mc_translate.c >exp-flowcheck/fc_translate.c
perl -pe 's/mc_/fc_/g; s/MC_/FC_/g; s/mcinclude/fcinclude/g; s/MEMCHECK/FLOWCHECK/g; s/MemCheck/FlowCheck/g; s/memcheck/flowcheck/g; s/MCE/FCE/g; s/mce/fce/g; s/\047M\047,/\047F\047,/g;' memcheck/memcheck.h >exp-flowcheck/flowcheck.h
mkdir exp-flowcheck/docs
touch exp-flowcheck/docs/Makefile.am
mkdir exp-flowcheck/tests
touch exp-flowcheck/tests/Makefile.am
