executable=$1
echo $executable
for dep in `ldd $executable | awk '{print $3}' | grep -v "("`
do
  echo "Copied dependency "$dep" to "$libsdir
done
