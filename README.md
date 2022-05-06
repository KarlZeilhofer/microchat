Microchat
=========

Einfaches Chat Programm über das lokale Netzwerk mittels UDP-Braodcasts auf Port 1111.
Als Sender Name wird der lokale Username des Betriebssystems verwendet.

## Gruppen
Eingehende Nachrichten werden nur angezeigt, wenn die Empfängergruppe in der
eigenen Liste angeführt ist und wenn die Nachricht entschlüsselt werden kann.

## Verschlüsselung
Es ist möglich, die Nachrichten zu verschlüsseln. Hierzu wird die Klasse "QAESEncryption" verwendet, siehe https://github.com/bricke/Qt-AES
Wird das Passwort leer gelassen, wird nicht verschlüsselt und somit im Klartext übertragen.
Die beiden Gruppen "Default" und "PlainText" existieren immer, wobei die erstere ein Standardpasswort verwendet.
Eine neue Gruppe kann angelegt werden, indem man in der Drop-Down-Box den neuen Namen eingibt und mit ENTER bestätigt.


## Mitwirkende
* Karl Zeilhofer
* mfian
* David Ecker
* Manuel Eder
* Christian Chimani
* Semin Begic
* Leonardo Alekov

