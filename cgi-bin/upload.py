import cgi
import cgitb
import os

cgitb.enable()


query = cgi.FieldStorage()
upload_dir = "./upload"
filename = query.getvalue('fichier')
email_address = query.getvalue('email')
upload_filehandle = query['fichier'].file


if not os.path.exists(upload_dir):
    os.makedirs(upload_dir)


with open(os.path.join(upload_dir, filename), 'wb') as upload_file:

    while True:
        buffer = upload_filehandle.read(1024)
        if not buffer:
            break
        upload_file.write(buffer)


body = f"""
<HTML>
<HEAD>
<TITLE>Telechargement Valide</TITLE>
</HEAD>

<BODY>

<P>Merci d'avoir téléversé votre photo !</P>
<P>Votre adresse email : {email_address}</P>
<P>Votre photo:</P>
<img src="{upload_dir}/{filename}" border="0">

</BODY>
</HTML>
"""


print("Content-Type: text/html; charset=utf-8")
print("Content-Length: {}".format(len(body)))
print("HTTP/1.1 200 OK")
# print()
print(body)