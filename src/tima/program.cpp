/* tima
 * Copyright (C) 2024 Killian Valverde.
 *
 * This file is part of tima.
 *
 * tima is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * tima is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with tima. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file        tima/program.cpp
 * @brief       program class implementation.
 * @author      Killian Valverde
 * @date        2024/07/17
 */
 
#include <alsa/asoundlib.h>

#include <cmath>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

#include "program.hpp"


namespace tima {


program::program(program_args&& prog_args) 
        : prog_args_(std::move(prog_args))
{
}


int program::execute()
{
    int retv = 0;
    
    time_attack(prog_args_.hors, prog_args_.mins, prog_args_.secs);
    
    for (size_t i = 0; i < 40; ++i)
    {
        play_beep();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    
    return retv;
}


// TODO: This code was generated, improve it.
void program::time_attack(std::uint64_t hours, std::uint64_t minutes, std::uint64_t seconds)
{
    using namespace std::chrono;

    // Calculate the total time in milliseconds
    std::uint64_t total_ms = (hours * 3600 + minutes * 60 + seconds) * 1000;
    
    auto start = steady_clock::now();
    auto end = start + milliseconds(total_ms);

    while (steady_clock::now() < end)
    {
        auto now = steady_clock::now();
        auto remaining = duration_cast<milliseconds>(end - now);

        // Calculate hours, minutes, seconds, and milliseconds remaining
        auto remaining_ms = remaining.count();
        std::uint64_t rem_hours = remaining_ms / 3600000;
        remaining_ms %= 3600000;
        std::uint64_t rem_minutes = remaining_ms / 60000;
        remaining_ms %= 60000;
        std::uint64_t rem_seconds = remaining_ms / 1000;
        remaining_ms %= 1000;

        // Display the remaining time
        std::cout << "\r"
                  << std::setw(2) << std::setfill('0') << rem_hours << ":"
                  << std::setw(2) << std::setfill('0') << rem_minutes << ":"
                  << std::setw(2) << std::setfill('0') << rem_seconds << "."
                  << std::setw(3) << std::setfill('0') << remaining_ms
                  << std::flush;

        // Sleep for 1 millisecond
        std::this_thread::sleep_for(milliseconds(2));
    }
    
    std::cout << "\n";
}


// TODO: This code was generated, improve it.
void program::play_beep()
{
    snd_pcm_t *handle;
    snd_pcm_sframes_t frames;
    int rc;
    int dir;
    snd_pcm_hw_params_t *params;
    unsigned int rate = 44100; // Sample rate
    int channels = 1;          // Mono sound
    snd_pcm_uframes_t frames_per_period = 32;

    // Open PCM device for playback.
    rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        std::cerr << "Unable to open PCM device: " << snd_strerror(rc) << std::endl;
        return;
    }

    // Allocate a hardware parameters object.
    snd_pcm_hw_params_malloc(&params);

    // Fill it in with default values.
    snd_pcm_hw_params_any(handle, params);

    // Set the desired hardware parameters.
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(handle, params, channels);
    snd_pcm_hw_params_set_rate_near(handle, params, &rate, &dir);
    snd_pcm_hw_params_set_period_size_near(handle, params, &frames_per_period, &dir);

    // Write the parameters to the driver.
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        std::cerr << "Unable to set HW parameters: " << snd_strerror(rc) << std::endl;
        return;
    }

    // Use a buffer large enough to hold one period.
    int size = frames_per_period * 2; // 2 bytes/sample, 1 channel
    char *buffer = (char *) malloc(size);

    // Generate a custom waveform similar to the Discord notification sound.
    int beep_duration = 300000; // Duration in microseconds (300ms)
    float frequency1 = 523.25;  // C5 note
    float frequency2 = 659.25;  // E5 note
    float amplitude = 8000.0;   // Reduced amplitude for a more pleasant sound
    int sample_rate = 44100;

    for (int i = 0; i < size / 2; i++) {
        float t = (float)i / sample_rate;
        float envelope = (1.0 - (float)i / (size / 2)); // Simple linear fade-out envelope
        float sample = amplitude * envelope * (0.5 * sin(2.0 * M_PI * frequency1 * t) + 0.5 * sin(2.0 * M_PI * frequency2 * t));
        int16_t int_sample = static_cast<int16_t>(sample);
        buffer[2 * i] = int_sample & 0xff;
        buffer[2 * i + 1] = (int_sample >> 8) & 0xff;
    }

    // Play the sound.
    for (int i = 0; i < beep_duration / (frames_per_period * 1000000 / sample_rate); ++i) {
        rc = snd_pcm_writei(handle, buffer, frames_per_period);
        if (rc == -EPIPE) {
            // EPIPE means underrun.
            std::cerr << "Underrun occurred" << std::endl;
            snd_pcm_prepare(handle);
        } else if (rc < 0) {
            std::cerr << "Error writing to PCM device: " << snd_strerror(rc) << std::endl;
        }
    }

    // Clean up.
    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    free(buffer);
}


}
