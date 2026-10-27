# entryhash.sh file first [last]: hash of the printed lines, as contents/contest/hash.sh computes it
for c in cpp-16 cpp-15 cpp-14 cpp; do command -v $c > /dev/null && break; done
sed -n "$2,${3:-\$}p" "$1" | $c -dD -P -fpreprocessed | tr -d '[:space:]' | md5sum | cut -c-6
