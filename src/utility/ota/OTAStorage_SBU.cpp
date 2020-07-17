/*
   This file is part of ArduinoIoTCloud.

   Copyright 2020 ARDUINO SA (http://www.arduino.cc/)

   This software is released under the GNU General Public License version 3,
   which covers the main part of arduino-cli.
   The terms of this license can be found at:
   https://www.gnu.org/licenses/gpl-3.0.en.html

   You can be released from the requirements of the above licenses by purchasing
   a commercial license. Buying such a license is mandatory if you want to modify or
   otherwise use the software for commercial activities involving the Arduino
   software without disclosing the source code of your own applications. To purchase
   a commercial license, send an email to license@arduino.cc.
*/

/******************************************************************************
   INCLUDE
 ******************************************************************************/

#include <AIoTC_Config.h>
#if OTA_STORAGE_SBU

#include "OTAStorage_SBU.h"

/******************************************************************************
   CONSTANTS
 ******************************************************************************/

static char const SBU_UPDATE_FILENAME[] = "UPDATE.BIN";     //.LZSS
static char const SBU_CHECK_FILE_NAME[] = "UPDATE.OK";

/******************************************************************************
   PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

bool OTAStorage_SBU::init()
{
  if (!_fileUtils.begin())
    return false;

  if (_fileUtils.listFile(SBU_UPDATE_FILENAME) > 0)
    if (!_fileUtils.deleteFile(SBU_UPDATE_FILENAME))
      return false;

  if (_fileUtils.listFile(SBU_CHECK_FILE_NAME) > 0)
    if (!_fileUtils.deleteFile(SBU_CHECK_FILE_NAME))
      return false;
}

bool OTAStorage_SBU::open()
{
  return true;
}

size_t OTAStorage_SBU::write(uint8_t const* const buf, size_t const num_bytes)
{
  _fileUtils.appendFile(SBU_UPDATE_FILENAME, (const char*)buf, num_bytes);
  return num_bytes;
}

void OTAStorage_SBU::close()
{
  /* Nothing to do */
}

void OTAStorage_SBU::remove()
{
  _fileUtils.deleteFile(SBU_UPDATE_FILENAME);
}

bool OTAStorage_SBU::rename()
{
  /* Create a file 'UPDATE.OK' which is used by the SBU
   * 2nd stage bootloader to recognise that the update
   * went okay. Normally this is done by renaming 'UPDATE.BIN.TMP'
   * to 'UPDATE.BIN' but the SARE module does not support
   * a rename function.
   */
  char c = 'X';
  return (_fileUtils.appendFile(SBU_CHECK_FILE_NAME, &c, sizeof(c)) == sizeof(c));
}

void OTAStorage_SBU::deinit()
{
  /* Nothing to do */
}

#endif /* OTA_STORAGE_SBU */
