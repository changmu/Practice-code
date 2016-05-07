# Cleanup
# Run as root, of course

cd $HOME/tmp
cat /dev/null > messages
cat /dev/null > wtmp
echo "Log files cleaned up."
