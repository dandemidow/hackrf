// dandemidow(c)

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <complex.h>
#include <variant>
#include <tuple>

#include <libhackrf/hackrf.h>

int tx_callback(hackrf_transfer* transfer) {
  /* Determine how many bytes we need to put in the buffer. */
  // bytes_to_read = transfer->buffer_length;
  /* Fill the buffer. */
  // transfer->buffer
  /* Now set the valid length to the bytes we put in the buffer. */
  // transfer->valid_length = bytes_read;
  
  /* Then return normally. */
	return 0;
}

volatile bool do_exit = false;

static constexpr double f_mod = 440;
static constexpr uint64_t sample_rate = 10000000;

int xfered_samples = 0;
static constexpr int samples_to_xfer = 5 * sample_rate;
volatile int should_stop = 0;

static constexpr int word_size = sample_rate / f_mod;
static size_t glob_index {};
static std::array<int8_t, word_size*20> signal_buffer {};

static void signal_init(int8_t *buffer, int size) {
    int i = 0;
    for (i = 0; i < size * word_size; i += 2) {
        buffer[i] = 1;
        buffer[i+1] = 0;
    }
    int start = i;
    static constexpr uint8_t kCoef {128U};
    for (; i < 2 * size * word_size; i += 2) {
        buffer[i] = kCoef * std::cos(6.28 * f_mod / sample_rate * (i - start));
        buffer[i+1] = kCoef * std::sin(6.28 * f_mod / sample_rate * (i - start));
    }
}

static void tx_complete_callback(hackrf_transfer* transfer, int success) {
	// If a transfer failed to complete
	if (!success) { }
    int8_t *signed_buffer = (int8_t*)transfer->buffer;
    for (int i = 0; i<transfer->buffer_length; i+=2) {
        signed_buffer[i] = signal_buffer[glob_index];
        signed_buffer[i+1] = signal_buffer[glob_index + 1];
        glob_index = (glob_index + 2) % signal_buffer.size();
    }
    transfer->valid_length = transfer->buffer_length;
}

int main() {
  int result;
  int txvga_gain = 20;

  signal_init(signal_buffer.data(), 5);

  hackrf_init();
  hackrf_device *_device {};
  hackrf_open(&_device);

  hackrf_set_freq(_device, 144500000);
  hackrf_set_sample_rate(_device, sample_rate);
  //hackrf_set_baseband_filter_bandwidth(_device, hackrf_compute_baseband_filter_bw_round_down_lt(10000000));
  hackrf_set_amp_enable(_device, 1);

  result = hackrf_set_txvga_gain(_device, txvga_gain);
  // hackrf_set_tx_underrun_limit(device, 100000); // new-ish library function, not always available
  //result |= hackrf_enable_tx_flush(device, flush_callback, NULL);
  //result |= hackrf_set_tx_block_complete_callback(_device, tx_complete_callback);
  result |= hackrf_start_tx(_device, tx_callback, NULL);
  
  
  result = hackrf_stop_tx(_device);
  if (result != HACKRF_SUCCESS) {
	fprintf(stderr, "hackrf_stop_tx() failed: %d\r\n",result);
  }

  // Spin until done or killed.
  while ((hackrf_is_streaming(_device) == HACKRF_TRUE ) &&
         (do_exit == false)) {
      sleep(1);
  }

  hackrf_close(_device);
  hackrf_exit();
	
  return 0;
}
