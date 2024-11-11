# Befehl-Referenz

Stelle sicher, dass Du die korrekte Syntax befolgst und die erforderlichen Argumente für jeden Befehl bereitstellst.

Du kannst diese Befehle in der `package.ini`-Datei konfigurieren, indem Du sie unter den entsprechenden Optionen angibst. Achte darauf, die notwendigen Argumente wie beschrieben für jeden Befehl bereitzustellen.

Ultrahand unterstützt derzeit die folgenden Befehle:

## Systembefehle

- `reboot`: Startet das System neu. Kann auch verwendet werden, um in Hekate / UMS oder Hekate ini Boot-Einträge nach Eintragsnamen neu zu starten.
  - Verwendung: `reboot` oder `reboot hekate` oder `reboot ums` oder
    - `reboot boot <NAME_DES_BOOT_EINTRAGS>` (für Einträge innerhalb von `hekate_ipl.ini`)
    - `reboot ini <NAME_DES_INI_EINTRAGS>` oder `reboot ini <NAME_DES_INI_EINTRAGS> <DUPLIKAT_INDEX>`

- `shutdown`: Fährt das System oder alle Bluetooth-Controller herunter.
  - Verwendung: `shutdown` oder `shutdown controllers`

- `backlight`: Ändert den aktuellen Wert der Hintergrundbeleuchtung.
  - Verwendung: `backlight on` oder `backlight off` oder `backlight <PROZENTZAHL_INTEGER>`

## Dateisystembefehle

- `make` oder `mkdir`: Erstellt ein Verzeichnis.
  - Verwendung: `mkdir <verzeichnis_pfad>`

- `copy` oder `cp`: Kopiert eine Datei oder ein Verzeichnis.
  - Verwendung: `copy <quelle_datei_pfad> <ziel_datei_pfad>`

- `mirror_copy` oder `mirror_cp`: Spiegelt den Inhalt eines Verzeichnisses.
  - Verwendung: `mirror_copy <quelle_datei_pfad> <ziel_datei_pfad>`

- `delete` oder `del`: Löscht eine Datei oder ein Verzeichnis.
  - Verwendung: `delete <datei_pfad>`

- `mirror_delete` oder `mirror_del`: Verwendet den Inhalt eines Quellverzeichnisses, um Dateien im Zielpfad zu entfernen.
  - Verwendung: `mirror_delete <quelle_datei_pfad> <ziel_datei_pfad>`

- `move` oder `mv`: Verschiebt/benennt eine Datei/ein Verzeichnis an einen neuen Ort/Label um.
  - Verwendung: `move <datei_pfad> <ziel_verzeichnis_pfad>`

- `download`: Lädt Dateien von einer angegebenen URL an ein lokales Ziel herunter.
  - Verwendung: `download <datei_url> <ziel_datei_pfad>`

- `unzip`: Extrahiert Dateien aus einem angegebenen ZIP-Archiv an ein lokales Ziel.
  - Verwendung: `unzip <zip_datei_pfad> <ziel_datei_pfad>`

## INI-Befehle

- `set-ini-val` oder `set-ini-value`: Bearbeitet/erstellt eine INI-Datei, indem ein Abschnitt mit einem gewünschten Schlüssel-Wert-Paar aktualisiert/hinzugefügt wird.
  - Verwendung: `set-ini-val <datei_zum_bearbeiten> <gewünschter_abschnitt> <gewünschter_schlüssel> <gewünschter_wert>`

- `set-ini-key`: Bearbeitet eine INI-Datei, indem ein Abschnitt mit einem neuen Schlüssel aktualisiert wird.
  - Verwendung: `set-ini-key <datei_zum_bearbeiten> <gewünschter_abschnitt> <gewünschter_schlüssel> <neuer_schlüssel>`

- `remove-ini-section`: Entfernt einen Abschnitt aus einer INI-Datei.
  - Verwendung: `remove-ini-section <ini_datei_pfad> <abschnittsname>`

## Hex-Bearbeitungsbefehle

- **hex-by-offset**
  - **Beschreibung**: Bearbeitet den Inhalt einer Datei an einem angegebenen Offset mit den bereitgestellten Hex-Daten.
  - **Verwendung**: `hex-by-offset <datei_pfad> <offset> <hex_daten>`

- **hex-by-custom-offset**
  - **Beschreibung**: Bearbeitet den Inhalt einer Datei an einem angegebenen benutzerdefinierten Muster-Offset mit den bereitgestellten Hex-Daten.
  - **Verwendung**: `hex-by-custom-offset <datei_pfad> <benutzerdefiniertes_muster> <offset> <hex_daten>`

- **hex-by-custom-decimal-offset**
  - **Beschreibung**: Bearbeitet den Inhalt einer Datei an einem angegebenen benutzerdefinierten Muster-Offset mit den bereitgestellten Dezimaldaten, die in Hex umgewandelt werden.
  - **Verwendung**: `hex-by-custom-decimal-offset <datei_pfad> <benutzerdefiniertes_muster> <offset> <dezimal_daten>`

- **hex-by-custom-rdecimal-offset**
  - **Beschreibung**: Bearbeitet den Inhalt einer Datei an einem angegebenen benutzerdefinierten Muster-Offset mit den bereitgestellten umgekehrten Dezimaldaten, die in umgekehrtes Hex umgewandelt werden.
  - **Verwendung**: `hex-by-custom-rdecimal-offset <datei_pfad> <benutzerdefiniertes_muster> <offset> <rdezimal_daten>`

- **hex-by-swap**
  - **Beschreibung**: Bearbeitet den Inhalt einer Datei, indem bestimmte Hex-Daten durch andere ersetzt werden. Du kannst optional ein Vorkommen angeben, das ersetzt werden soll.
  - **Verwendung**: `hex-by-swap <datei_pfad> <hex_daten_zum_ersetzen> <hex_daten_ersatz> [vorkommen]`

- **hex-by-string**
  - **Beschreibung**: Bearbeitet den Inhalt einer Datei, indem bestimmte String-Daten durch andere ersetzt werden. String-Daten werden automatisch in Hex umgewandelt.
  - **Verwendung**: `hex-by-string <datei_pfad> <string_daten_zum_ersetzen> <string_daten_ersatz> [vorkommen]`

- **hex-by-decimal**
  - **Beschreibung**: Bearbeitet den Inhalt einer Datei, indem bestimmte Dezimaldaten durch andere ersetzt werden. Dezimaldaten werden vor dem Ersetzen in Hex umgewandelt.
  - **Verwendung**: `hex-by-decimal <datei_pfad> <dezimal_daten_zum_ersetzen> <dezimal_daten_ersatz> [vorkommen]`

- **hex-by-rdecimal**
  - **Beschreibung**: Bearbeitet den Inhalt einer Datei, indem bestimmte umgekehrte Dezimaldaten durch andere ersetzt werden. Dezimaldaten werden vor dem Ersetzen in umgekehrtes Hex umgewandelt.
  - **Verwendung**: `hex-by-rdecimal <datei_pfad> <dezimal_daten_zum_ersetzen> <dezimal_daten_ersatz> [vorkommen]`

- **hex-by-pattern**
  - **Beschreibung**: Bearbeitet den Inhalt einer Datei, indem Daten ersetzt werden, die einem bestimmten Hex-Muster entsprechen.
  - **Verwendung**: `hex-by-pattern <datei_pfad> <hex_muster_zum_ersetzen> <hex_daten_ersatz>`

---

### Hinweise

- **Vorkommen**: Einige Funktionen erlauben die Angabe eines optionalen `vorkommen`-Parameters, der definiert, welches Vorkommen des Musters oder der Daten ersetzt wird. Wenn nicht angegeben, werden alle Vorkommen ersetzt.
- **Hex-Konvertierungen**: Funktionen wie `hex-by-decimal` und `hex-by-string` konvertieren Dezimal- und String-Daten automatisch in Hex-Format, bevor die Ersetzung in der Datei erfolgt.

## Mod-Konvertierungsbefehle

- `pchtxt2ips`: Konvertiert eine .pchtxt-Mod in eine .ips.
  - Verwendung: `pchtxt2ips <pchtxt_datei_pfad> <ausgabe_datei_pfad>`

- `pchtxt2cheat`: Konvertiert und installiert eine .pchtxt-Mod in einen Cheat.
  - Verwendung: `pchtxt2cheat <pchtxt_datei_pfad>`

## Sourced Placeholder-Funktionen

- `*` (Sternchen):
  - Das Sternchen * wird oft als Platzhalter oder Wildcard verwendet und repräsentiert einen Wert, der während der Ausführung dynamisch gefüllt wird. Es wird häufig in Kontexten verwendet, in denen spezifische Werte erwartet werden, und der tatsächliche Wert wird zur Laufzeit bestimmt.
- `package_source`:
  - Package Source ermöglicht es Benutzern, ein Ultrahand-Paket-ini von einem anderen weiterzuleiten. Dies kann Benutzern helfen, ihren Code zu organisieren und längere Menüs zu verketten.
  - Verwendet mit `;mode=forwarder`, genannt `package_source /pfad/zum/forwarder_package.ini`.
- `list_source`:
  - List Source ist eine Möglichkeit, Variablen für Dropdown-Menüs innerhalb der package.ini zu speichern. Es wird als Platzhalter für die einzelnen Werte innerhalb der List Source-Variable verwendet. Der Code ersetzt List Source-Referenzen durch den entsprechenden Wert.
- `list_file_source`:
  - Ähnlich wie `list_source`, verwendet jedoch ein Textdateiverzeichnis als Eingabe, wobei die Textdatei jeden Eintrag durch Zeilenumbrüche trennt.
- `file_source`:
  - Darstellung einer Quelldatei oder Ressource. Es ist ein Platzhalter für den tatsächlichen Dateinamen oder den Quellort. Der Code ersetzt die Quelle durch den entsprechenden Dateipfad, wenn bestimmte Befehle ausgeführt werden.

    Hier ist eine Aufschlüsselung der zusätzlichen Variablen, die durch die Verwendung von `file_source` zugänglich sind:
    - `file_name`: Der Name der einzelnen Datei.
    - `folder_name`: Der Name des übergeordneten Ordners der Datei.

  - `file_source` in einem `on:`-Abschnitt impliziert, dass ein Befehl oder eine Aktion ausgeführt werden soll, wenn eine bestimmte Quelle "on" oder aktiviert ist.
  - `file_source` in einem `off:`-Abschnitt impliziert, dass ein Befehl oder eine Aktion ausgeführt werden soll, wenn eine bestimmte Quelle "off" oder deaktiviert ist.
  - Bewertet den Zustand eines Schalters oder einer Bedingung und wählt dann den entsprechenden Befehl zur Ausführung basierend darauf aus, ob die Quelle ein- oder ausgeschaltet ist.
  - Für `file_source`-Funktionstoggles, siehe bitte das [Broomstick](https://github.com/ppkantorski/Ultrahand-Overlay/tree/main/examples/Broomstick)-Paketbeispiel.
- `ini_file_source`:
  - Ähnlich wie `list_file_source`, verwendet jedoch Eintragsnamen aus einer ini-Datei für seinen Listenwert. Es behält auch andere ini-Funktionen ähnlich wie `ini_file`.
- `json_file_source`:
  - Dieser Befehl wird verwendet, um eine JSON-Datenquelle zu definieren. Er gibt einen Dateipfad zu einer JSON-Datei an, die Daten enthält. Diese JSON-Datenquelle wird für verschiedene Zwecke innerhalb des Codes verwendet.
  - Der Code liest Daten aus der angegebenen JSON-Datei und führt basierend auf dem Kontext und der Logik im Code möglicherweise Aktionen oder Operationen mit diesen Daten aus.
  - In einigen Fällen kann es auch ein zusätzliches Argument (z.B. jsonKey) akzeptieren, um einen bestimmten Schlüssel oder eine Eigenschaft innerhalb der JSON-Daten anzugeben, die für die Anzeige oder Verarbeitung verwendet werden soll.
  - Dieser Befehl wird verwendet, um strukturierte Daten aus JSON-Dateien abzurufen und in die Funktionalität des Programms zu integrieren.

    Hier ist eine Aufschlüsselung der zusätzlichen Variablen, die durch die Verwendung von `json_file_source` zugänglich sind:
  - `jsonPath`: Der Dateipfad zur JSON-Datenquelle.
  - `jsonKey`: Ein optionaler Schlüssel innerhalb der JSON-Daten für spezifische Datenabrufung.

  - Für Informationen zu `json_file_source`-Funktionsimplementierungen, siehe bitte das [Easy Installer](https://github.com/glitched-nx/ultrahand/tree/main/examples/Easy%20Installer)-Paketbeispiel.

## Platzhalterfunktionen

- `{ram_vendor}`
  - **Beschreibung**: Gibt den Hersteller des RAMs des Geräts zurück.
  - **Beispiel**: Zeigt "Samsung" für ein Gerät mit Samsung RAM an.

- `{ram_model}`
  - **Beschreibung**: Gibt die Modellnummer des RAMs im Gerät zurück.
  - **Beispiel**: Zeigt "M471A1K43BB1-CTD" als Modellnummer an.

- `{ams_version}`
  - **Beschreibung**: Gibt die auf dem Gerät installierte Atmosphere-Version zurück.
  - **Beispiel**: Zeigt "1.7.1" für Atmosphere-Version 1.7.1 an.

- `{hos_version}`
  - **Beschreibung**: Gibt die Horizon OS-Version zurück.
  - **Beispiel**: Zeigt "18.1.0" für Horizon OS-Version 18.1.0 an.

- `{cpu_speedo}`
  - **Beschreibung**: Gibt den CPU-Speedo-Wert des Geräts zurück.
  - **Beispiel**: Zeigt "1600" für den CPU-Speedo-Wert des Geräts an.

- `{cpu_iddq}`
  - **Beschreibung**: Gibt den CPU-IDDQ-Wert des Geräts zurück.
  - **Beispiel**: Zeigt "60" für den CPU-IDDQ-Wert des Geräts an.

- `{gpu_speedo}`
  - **Beschreibung**: Gibt den GPU-Speedo-Wert des Geräts zurück.
  - **Beispiel**: Zeigt "1600" für den GPU-Speedo-Wert des Geräts an.

- `{gpu_iddq}`
  - **Beschreibung**: Gibt den GPU-IDDQ-Wert des Geräts zurück.
  - **Beispiel**: Zeigt "60" für den GPU-IDDQ-Wert des Geräts an.

- `{soc_speedo}`
  - **Beschreibung**: Gibt den SOC-Speedo-Wert des Geräts zurück.
  - **Beispiel**: Zeigt "1600" für den SOC-Speedo-Wert des Geräts an.

- `{soc_iddq}`
  - **Beschreibung**: Gibt den SOC-IDDQ-Wert des Geräts zurück.
  - **Beispiel**: Zeigt "60" für den SOC-IDDQ-Wert des Geräts an.

- `{slice(<STRING>,<START_INDEX>,<END_INDEX>)}`
  - **Beschreibung**: Schneidet einen String basierend auf den angegebenen Start- und Endindizes.
  - **Parameter**:
    - `<STRING>`: Der zu schneidende String.
    - `<START_INDEX>`: Der Startindex des Schnitts.
    - `<END_INDEX>`: Der Endindex des Schnitts.
  - **Beispiel**: `{slice("Test String", 0, 4)}` gibt "Test" zurück.

- `{decimal_to_hex(<DECIMAL>)}`
  - **Beschreibung**: Konvertiert eine Dezimalzahl in einen Hexadezimal-String.
  - **Parameter**:
    - `<DECIMAL>`: Die zu konvertierende Dezimalzahl.
  - **Beispiel**: `{decimal_to_hex(255)}` gibt "FF" zurück.

- `{ascii_to_hex(<ASCII>)}`
  - **Beschreibung**: Konvertiert einen ASCII-String in seine hexadezimale Darstellung.
  - **Parameter**:
    - `<ASCII>`: Der zu konvertierende ASCII-String.
  - **Beispiel**: `{ascii_to_hex("Test")}` gibt "54657374" zurück.

- `{hex_to_rhex(<HEX>)}`
  - **Beschreibung**: Konvertiert einen hexadezimalen String in umgekehrtes Endian-Hex.
  - **Parameter**:
    - `<HEX>`: Der zu konvertierende hexadezimale String.
  - **Beispiel**: `{hex_to_rhex("12345678")}` gibt "78563412" zurück.

- `{hex_to_decimal(<HEX>)}`
  - **Beschreibung**: Konvertiert einen hexadezimalen String in seine dezimale Darstellung.
  - **Parameter**:
    - `<HEX>`: Der zu konvertierende hexadezimale String.
  - **Beispiel**: `{hex_to_decimal("FF")}` gibt "255" zurück.

- `{split(<STRING>,<PATTERN>,<INDEX>)}`
  - **Beschreibung**: Teilt einen String basierend auf einem Muster und gibt einen bestimmten Teil zurück.
  - **Parameter**:
    - `<STRING>`: Der zu teilende String.
    - `<PATTERN>`: Das Muster, das zum Teilen des Strings verwendet wird.
    - `<INDEX>`: Der Index des zurückzugebenden geteilten Teils.
  - **Beispiel**: `{split("Test - String", " - ", 0)}` gibt "Test" zurück.

- `{value}`
  - **Beschreibung**: Gibt den aktuellen Wert eines Schiebereglers oder Trackbars zurück.
  - **Beispiel**: Wird in Schiebereglern verwendet, um den aktuell ausgewählten Wert anzuzeigen.

- `{index}`
  - **Beschreibung**: Gibt den aktuellen Index einer Auswahl oder Liste zurück.
  - **Beispiel**: Wird verwendet, um die Position eines Elements in einer Liste oder Dropdown anzuzeigen.

---

- `{list_source(<INDEX>)}`
  - **Beschreibung**: Ruft ein Element aus einer zuvor mit `list_source` definierten Liste ab.
  - **Parameter**:
    - `<INDEX>`: Index des Elements in der Liste.
  - **Beispiel**: `{list_source(0)}` ruft das erste Element in der Liste ab.

- `{json_source(<KEY>)}`
  - **Beschreibung**: Ruft einen Wert aus einer JSON-Quelle basierend auf dem angegebenen Schlüssel ab.
  - **Parameter**:
    - `<KEY>`: Der Schlüssel, der im JSON-Objekt nachgeschlagen werden soll.
  - **Beispiel**: `{json_source("title")}` ruft den Wert ab, der mit dem Schlüssel "

"title" im JSON-Objekt verknüpft ist.

- `{ini_file(<SECTION>, <KEY>)}`
  - **Beschreibung**: Gibt den Wert eines Schlüssels in einem bestimmten Abschnitt einer INI-Datei zurück.
  - **Parameter**:
    - `<SECTION>`: Der Abschnitt in der INI-Datei.
    - `<KEY>`: Der Schlüssel innerhalb des Abschnitts.
  - **Beispiel**: `{ini_file("Settings", "Language")}` gibt den Wert für den Schlüssel "Language" im Abschnitt "Settings" zurück.

## Befehlsmodi

- `;mode=default`
  - **Beschreibung**: Der Standardbefehlsmodus, der den Befehl so ausführt, wie er ist, ohne spezielle Interaktion oder Verhalten. Wird verwendet, wenn kein anderer Modus explizit erforderlich ist.
  - **Verwendung**: Wenn ein Befehl in seiner Standardform ausgeführt wird.
  - **Beispiel**:

      ```ini
      [Reboot]
      ;mode=default
      reboot
      ```

- `;mode=toggle`
  - **Beschreibung**: Ein Umschaltbefehl, der zwischen zwei Zuständen (ein/aus) wechselt. Der Befehl wird basierend auf seinem aktuellen Zustand ausgeführt und nach der Ausführung in den anderen Zustand umgeschaltet.
  - **Verwendung**: Nützlich für Einstellungen oder Funktionen, die aktiviert/deaktiviert werden können.
  - **Parameter**:
    - `toggle?on`: Setzt das Umschalten standardmäßig auf `ein`.
    - `toggle?off`: Setzt das Umschalten standardmäßig auf `aus`.
  - **Beispiel**:

      ```ini
      [Backlight]
      ;mode=toggle?on
      on:
      backlight on
      off:
      backlight off
      ```

- `;mode=option`
  - **Beschreibung**: Zeigt eine Liste von Optionen an, aus denen der Benutzer wählen kann. Dieser Modus ermöglicht es dem Benutzer, eine von mehreren vordefinierten Optionen auszuwählen.
  - **Verwendung**: Nützlich für Befehle, die mehrere Optionen zur Auswahl benötigen, wie z.B. Konfigurationen oder Einstellungen.
  - **Beispiel**:

      ```ini
      [Graphics Settings]
      ;mode=option
      list_source '(Low, Medium, High)'
      ```

- `;mode=trackbar`
  - **Beschreibung**: Erstellt einen Standard-Trackbar (Schieberegler), der es dem Benutzer ermöglicht, einen Wert zwischen einem minimalen und maximalen Bereich anzupassen.
  - **Verwendung**: Für numerische Einstellungen wie Lautstärke, Helligkeit oder Leistungsanpassungen.
  - **Parameter**:
    - `min_value`: Der minimale ganzzahlige Wert des Trackbars.
    - `max_value`: Der maximale ganzzahlige Wert des Trackbars.
    - `units`: Anzeigeeinheiten für den Wert.
    - `unlocked`: Ermöglicht optional das sofortige Interagieren mit dem Trackbar.
    - `on_every_tick`: Führt optional den Befehl bei jedem Trackbar-Tick aus (standardmäßig deaktiviert).
  - **Beispiel**:

      ```ini
      [Volume]
      ;mode=trackbar
      min_value=0
      max_value=100
      units=%
      ```

- `;mode=step_trackbar`
  - **Beschreibung**: Ähnlich wie der reguläre `trackbar`, jedoch mit festen Schritten zwischen den Werten. Anstatt den Wert frei anzupassen, bewegt sich der Benutzer durch vordefinierte Intervalle.
  - **Verwendung**: Nützlich, wenn Du einen Schieberegler mit bestimmten Schritten möchtest.
  - **Parameter**:
    - `steps`: Definiert die Anzahl der Schritte, die der Trackbar haben wird.
  - **Beispiel**:

      ```ini
      [Brightness]
      ;mode=step_trackbar
      min_value=0
      max_value=5
      steps=6
      ```

- `;mode=named_step_trackbar`
  - **Beschreibung**: Ein Step-Trackbar, der vordefinierte Namen für jeden Schritt verwendet. Der Benutzer wählt einen Schritt aus einer Liste benannter Optionen aus.
  - **Verwendung**: Wird verwendet, wenn spezifische Labels oder benannte Schritte erforderlich sind, wie z.B. Qualitätseinstellungen (Low, Medium, High).
  - **Parameter**:
    - `list_source`: Die Quellliste der Namen für jeden Schritt.
  - **Beispiel**:

      ```ini
      [Performance Mode]
      ;mode=named_step_trackbar
      list_source '(Low, Medium, High, Ultra)'
      ```

- `;mode=slot`
  - **Beschreibung**: Slot-Modus ermöglicht es dem Benutzer, Befehle aus einem vordefinierten Satz von Optionen mit benutzerdefinierten Labels und Fußzeilen auszuführen.
  - **Verwendung**: Wird verwendet, wenn Befehle aus einer Gruppe ausgewählt werden müssen, ähnlich wie `option`, jedoch mit mehr Kontrolle über Anzeigeelemente.
  - **Beispiel**:

      ```ini
      [*Save Slot]
      ;mode=slot
      list_source '(test a, test b, test c)'
      set_footer list_source(*)
      ```

- `;mode=forwarder`
  - **Beschreibung**: Ein Weiterleitungsbefehl, der auf die `.ini`-Datei eines anderen Pakets verweist. Der Benutzer wählt den Weiterleiter aus und wird zu dem weitergeleiteten Paket geführt oder führt Befehle aus diesem Paket aus.
  - **Verwendung**: Nützlich zum Verknüpfen von Befehlen oder Paketen auf modulare Weise.
  - **Parameter**:
    - `package_source`: Verweist auf die weitergeleitete Paket-`.ini`-Datei.
  - **Beispiel**:

      ```ini
      [Performance Options]
      ;mode=forwarder
      package_source '/switch/.packages/performance_config.ini'
      ```

- `;mode=table`
  - **Beschreibung**: Ein Tabellenanzeigemodus, der das Rendern von strukturierten Informationen in einem tabellenähnlichen Format mit Zeilen und Spalten ermöglicht.
  - **Verwendung**: Nützlich zum Anzeigen organisierter Daten wie Systeminformationen, Gerätespezifikationen usw.
  - **Parameter**:
    - `alignment`: Richtet Text innerhalb der Tabellenspalten aus (`left`, `center`, `right`).
    - `header`: Zeichnet einen Header für die Tabelle (true/false).
    - `spacing`: Legt den Abstand zwischen den Zeilen fest.
    - `gap`: Legt den Abstand nach der Tabelle fest.
  - **Beispiel**:

      ```ini
      [System Info]
      ;mode=table
      alignment=left
      header=true
      ```
