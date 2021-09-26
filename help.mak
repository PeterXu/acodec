# get makefile
_makefile_ = $(firstword $(MAKEFILE_LIST))

#.DEFAULT_GOAL := help
.PHONY: help
help:
	@echo "Targets in $(_makefile_):"
	@LC_ALL=C $(MAKE) -pqf $(_makefile_) 2>/dev/null | grep -i "^[a-z]\+[a-z0-9]\+:" \
		| grep -v "^$(_makefile_)\|^help" | sort \
		| awk -F: 'BEGIN{no=0}{print "  [" no "]: " $$1; no+=1;}'

