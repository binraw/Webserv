#!/usr/bin/perl -w

use CGI;

$query = new CGI;

$upload_dir = "./upload"; # a voir si on veut le mettre ailleurs
$filename = $query->param("") #ici mettre le nom de la balise
$email_adress = $query->param("") #ici aussi 
$upload_filehandle = $query->upload("")

open UPLOADFILE, ">$upload_dir/$filename";

while (<$upload_filehandle>)
{
    print UPLOADFILE;
}
close UPLOADFILE;

print $query->header ( ); #ici je ne sais pas si c'est a definir ou si il existe deja une value
print <<END_HTML;

<HTML>
<HEAD>
<TITLE>Thanks!</TITLE>
</HEAD>

<BODY>

<P>Thanks for uploading your photo!</P>
<P>Your email address: $email_address</P>
<P>Your photo:</P>
<img src="/upload/$filename" border="0">

</BODY>
</HTML>

END_HTML

