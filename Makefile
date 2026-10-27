# make test                     all tests (yukicoder ones need YUKICODER_TOKEN in .env or the env)
# make test ARGS="flow dinitz"  one section or one test
# make test COEF=2              multiply every time limit by 2 (slower machine)
# make pdf                      build tex/main.pdf
# make yukitoken <TOKEN>        save the yukicoder API token to .env (also: make yukitoken TOKEN=...)
.PHONY: test pdf yukitoken

# "make yukitoken abc": take the word after the goal as TOKEN and make it a no-op target
ifeq (yukitoken,$(firstword $(MAKECMDGOALS)))
  TOKEN ?= $(word 2,$(MAKECMDGOALS))
  ifneq ($(TOKEN),)
    $(eval $(TOKEN):;@:)
  endif
endif

test:
	cd testing && python3 all.py $(ARGS)

pdf:
	cd tex && latexmk -pdf -shell-escape -interaction=nonstopmode -halt-on-error main.tex

yukitoken:
	@test -n "$(TOKEN)" || { echo "usage: make yukitoken <TOKEN>"; exit 1; }
	@touch .env; grep -v '^YUKICODER_TOKEN=' .env > .env.tmp || true
	@echo 'YUKICODER_TOKEN=$(TOKEN)' >> .env.tmp; mv .env.tmp .env; chmod 600 .env
	@echo "saved YUKICODER_TOKEN to .env"
