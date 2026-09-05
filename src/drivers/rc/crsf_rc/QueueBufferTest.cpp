/****************************************************************************
 *
 *   Copyright (c) 2026 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include <gtest/gtest.h>

#include "QueueBuffer.hpp"

#include <array>
#include <cstdint>

TEST(QueueBufferTest, PeekHandlesWrappedIndex)
{
	std::array<uint8_t, 5> storage{};
	crsf::QueueBuffer queue;
	queue.initialize(storage.data(), storage.size());

	const std::array<uint8_t, 4> initial_data{1, 2, 3, 4};
	ASSERT_TRUE(queue.append(initial_data.data(), initial_data.size()));
	queue.dequeue(initial_data.size());

	const std::array<uint8_t, 5> wrapped_data{5, 6, 7, 8, 9};
	ASSERT_TRUE(queue.append(wrapped_data.data(), wrapped_data.size()));

	std::array<uint8_t, 2> result{};
	ASSERT_TRUE(queue.peek(2, result.data(), result.size()));
	EXPECT_EQ(result[0], 7);
	EXPECT_EQ(result[1], 8);
}

TEST(QueueBufferTest, AppendRejectsInsufficientSpace)
{
	std::array<uint8_t, 4> storage{};
	crsf::QueueBuffer queue;
	queue.initialize(storage.data(), storage.size());

	const std::array<uint8_t, 5> data{1, 2, 3, 4, 5};
	EXPECT_FALSE(queue.append(data.data(), data.size()));
	EXPECT_TRUE(queue.empty());
}
