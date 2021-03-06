/*
 * Copyright 2003-2016 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"
#include "PcmDsd.hxx"
#include "dsd2pcm/dsd2pcm.h"
#include "util/ConstBuffer.hxx"

#include <assert.h>

PcmDsd::PcmDsd()
{
	dsd2pcm.fill(nullptr);
}

PcmDsd::~PcmDsd()
{
	for (auto i : dsd2pcm)
		if (i != nullptr)
			dsd2pcm_destroy(i);
}

void
PcmDsd::Reset()
{
	for (auto i : dsd2pcm)
		if (i != nullptr)
			dsd2pcm_reset(i);
}

ConstBuffer<float>
PcmDsd::ToFloat(unsigned channels, ConstBuffer<uint8_t> src)
{
	assert(!src.IsNull());
	assert(!src.IsEmpty());
	assert(src.size % channels == 0);
	assert(channels <= dsd2pcm.max_size());

	const unsigned num_samples = src.size;
	const unsigned num_frames = src.size / channels;

	float *dest = buffer.GetT<float>(num_samples);

	for (unsigned c = 0; c < channels; ++c) {
		if (dsd2pcm[c] == nullptr) {
			dsd2pcm[c] = dsd2pcm_init();
			if (dsd2pcm[c] == nullptr)
				return nullptr;
		}

		dsd2pcm_translate(dsd2pcm[c], num_frames,
				  src.data + c, channels,
				  false, dest + c, channels);
	}

	return { dest, num_samples };
}

/**
 * Construct a 32 bit integer from four bytes.
 */
static constexpr inline uint32_t
Construct32(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	return uint32_t(a) | (uint32_t(b) << 8) |
		(uint32_t(c) << 16) | (uint32_t(d) << 24);
}

static constexpr inline uint32_t
Dsd8To32Sample(const uint8_t *src, unsigned channels)
{
	return Construct32(src[0], src[channels],
			   src[2 * channels], src[3 * channels]);
}

ConstBuffer<uint32_t>
Dsd8To32(PcmBuffer &buffer, unsigned channels, ConstBuffer<uint8_t> _src)
{
	const size_t in_frames = _src.size / channels;
	const size_t out_frames = in_frames / 4;
	const size_t out_samples = out_frames * channels;

	const uint8_t *src = _src.data;
	uint32_t *const dest0 = buffer.GetT<uint32_t>(out_samples);
	uint32_t *dest = dest0;

	for (size_t i = 0; i < out_frames; ++i) {
		for (size_t c = 0; c < channels; ++c)
			*dest++ = Dsd8To32Sample(src++, channels);

		src += 3 * channels;
	}

	return {dest0, out_samples};
}
