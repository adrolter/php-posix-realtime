--TEST--
Check for posixclocks presence
--SKIPIF--
<?php if (!extension_loaded("posixclocks")) print "skip"; ?>
--FILE--
<?php 
echo "posixclocks extension is available";
?>
--EXPECT--
posixclocks extension is available
