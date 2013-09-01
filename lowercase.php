<?php

$dir = $argv[1];

$list = scandir($dir);

foreach ($list as $file) {
    if ($file != '..' && $file != '.') {
       $lower = strtolower($file);
       rename($dir . "\\" . $file, $dir . "\\" . $lower);
    }
}



?>