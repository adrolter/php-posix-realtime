--TEST--
Check for posixrealtime presence
--SKIPIF--
<?php if (!extension_loaded("posixrealtime")) print "skip"; ?>
--FILE--
<?php 
echo "posixrealtime extension is available";
?>
--EXPECT--
posixrealtime extension is available
