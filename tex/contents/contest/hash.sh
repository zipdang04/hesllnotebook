# sh hash.sh < file: the hash at the right of an entry
# title; ignores whitespace and comments
cpp -dD -P -fpreprocessed | tr -d '[:space:]' |
	md5sum | cut -c-6
