#include "psg.h"
#include <osbind.h>

#define NOISE_REG 0x6
#define SETTINGS_REG 0x7
#define VOLUME_REG 0x8
#define ENV_PERIOD_REG 0xB
#define ENV_SHAPE_REG 0xD

/* ==== Function: read_psg ====
Purpose: Attempts to read a value from the given PSG register (0-15). This is a helper
         routine that will be helpful to test the other routines in this module.

Input: Register(UINT8) - The register that is being read from
       Value(UINT8&) - The value that was read from the register
                       Value is valid, if and only if the output is true

Output: Boolean indicating if a value was successfully read or not
*/
bool read_psg(UINT8 reg, UINT8 *value)
{
	UINT32 exit;

	if (reg & 0xF0) return false; /* Reject out of bounds read */

	exit = Super(0); /* Save old super sp */
	*(volatile char *)(0xFF8802) = reg; /* Select register */
	*value = *(volatile char *)0xFF8800; /* Get value */
	Super(exit);
	
	return true;
}

/* ==== Function: write_psg ====
Purpose: Writes the given byte value (0-255) to the given PSG register (0-15). This is a helper
         routine to be used by the other functions in this module.

Input: Register(UINT8) - Register that is being written to
       Value(UINT8) - Value being written to reg

Output: None
*/
void write_psg(UINT8 reg, UINT8 val)
{
	UINT32 exit;

	if (reg & 0xF0) return; /* Reject out of bounds read */

	exit = Super(0); /* Save old super sp */

	*(volatile char *)(0xFF8800) = reg; /* Select register */
	*(volatile char *)(0xFF8802) = val; /* Set value */

	Super(exit);
}

/* ==== Function: set_tone ====
Purpose: Loads the tone registers (coarse and fine) for the given channel (0=A, 1=B, 2=C)
         with the given {12-bit} tuning.

Input: Channel(UINT8) - The channel (A, B, or C) that is being written to
       Tuning(UINT16) - 12-bit tone that will be written to the specified channel

Output: None
*/
void set_tone(UINT8 channel, UINT16 tuning)
{
	UINT8 reg = (1<<channel) & 0x06; /* Values 0, 2, 4 */

	if (channel & 0xFC) return; /* Reject invalid channel */
	if (tuning & 0xF000) return; /* Reject invalid tuning */

	write_psg(channel, (UINT8)(tuning & 0x00FF)); /* Write fine tone adjustment */
	write_psg(channel+1, (UINT8)((tuning & 0x0F00)>>8)); /* Write coarse tone adjustment */
}

/* ==== Function: set_volume ====
Purpose: Set the volume level for the given channel

Input: Channel(UINT8) - The channel (A, B, or C) that is being written to
       Volume(UINT8) - The volume value to be written to specified channel

Output: None
*/
void set_volume(UINT8 channel, UINT8 volume)
{
	UINT8 reg = channel + VOLUME_REG;

	if (channel & 0xFC) return; /* Reject invalid channel */
	if (volume & 0xE0) return; /* Reject invalid volume */

	write_psg(reg, volume & 0x1F);
}

/* ==== Function: enable_channel ====
Purpose: Turns the given channel’s (0=A, 1=B, 2=C) tone/noise signals on/off (0=off, 1=on).

Input: Channel(UINT8) - The channel (A, B, or C) that is being written to
       Tone(UINT8) - Set the tone for the specified channel to be either on or off
       Noise(UINT8) - Set the noise for the specified channel to be either on or off

Output: None
*/
void enable_channel(UINT8 channel, UINT8 tone_on, UINT8 noise_on)
{
	UINT8 settings;
	if (channel & 0xFC) return; /* Reject invalid channel */

	read_psg(SETTINGS_REG, &settings); /* Get contents of settings register */

	/* 1 = off for PSG settings */
	if (tone_on == 0)
		settings |= (1<<channel);
	else
		settings &= ~(1<<channel);

	if (noise_on == 0)
		settings |= (1<<(channel+3));
	else
		settings &= ~(1<<(channel+3));

	write_psg(SETTINGS_REG, settings);
}

/* ==== Function: set_noise ====
Purpose: Load the Noise register with the given tuning.

Input: Tuning(UINT8) - The 5-bit tuning value loaded to the noise register

Output: None
*/
void set_noise(UINT8 tuning)
{
	if (tuning & 0xE0) return; /* Reject invalid tuning */

	write_psg(NOISE_REG, tuning & 0x1F);
}

/* ==== Function: set_envelope ====
Purpose: Loads the PSG envelope control registers with the given envelope shape and 16-bit sustain.

Input: Shape(UINT8) - The shape loaded into the envelope control registers
       Sustain(UINT16) - The sustain value being written to the envelope control registers

Output: None
*/
void set_envelope(UINT8 shape, UINT16 sustain)
{
	if (shape & 0xF0) return; /* Reject invalid shape */

	write_psg(ENV_PERIOD_REG, sustain);
	write_psg(ENV_SHAPE_REG, shape & 0x0F);
}

/* ==== Function: stop_sound ====
Purpose: Silences all PSG sound production.

Input: None

Output: None
*/
void stop_sound()
{
	UINT8 settings;
	read_psg(SETTINGS_REG, &settings); /* Get contents of settings register */

	/* 1 = off for PSG settings */
	settings &= 0x3F; /* Disable all channels */

	write_psg(SETTINGS_REG, settings);
}