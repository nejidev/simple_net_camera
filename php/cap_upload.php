<?php
/**
 * This file implements cap image upload.
 *
 * @author     nejidev
 * @since      2020-10-01 10:36
 */
date_default_timezone_set('PRC');

$allow_ext = array('.jpg','.gif','.png','.bmp','.jpeg');

function upLoad($name, $allow_ext)
{
	$file_name = $_FILES[$name]['name'];
	$ext_name  = substr($file_name, strrpos($file_name, '.'));
	$ext_name  = strtolower($ext_name);

	if(! in_array($ext_name, $allow_ext))
	{
		exit('not allow ext file');
	}

	$new_file_name = date('Y-m-d-H-i-s-').mt_rand(1000, 9999).$ext_name;
	move_uploaded_file($_FILES[$name]['tmp_name'], $new_file_name);

	return $new_file_name;
}

if(!empty($_FILES))
{
	echo upLoad('file', $allow_ext);
	exit;
}
?>
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=utf-8"/>
<title>file upload</title>
</head>
<body>

<form action="" enctype="multipart/form-data" method="post">
<input type="file" name="file">
<input type="submit" name="Submit">
</form>

</body>
</html>
