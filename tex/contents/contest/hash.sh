# sh hash.sh < lines: hash of the PRINTED lines (no #pragma
# once/#include) = the one by the title; GNU cpp, no ws/comments
cpp -dD -P -fpreprocessed | tr -d '[:space:]' |
	md5sum | cut -c-6
