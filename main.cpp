// dandemidow(c)

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <libhackrf/hackrf.h>

int rx_callback(hackrf_transfer *transfer) {
  void *hrf = (void *)transfer->rx_ctx;

  transfer->buffer;
  transfer->valid_length;
  fprintf(stderr, "rx [%p, %d]\r\n", transfer->buffer, transfer->valid_length);

  return 0;
}

int main() {

  int status {-1};
  hackrf_device *_device {};

  status = hackrf_init();
  if (status != HACKRF_SUCCESS) {
	fprintf(stderr, "Failed to initialize HackRf.\r\n");
  }

  status = hackrf_open(&_device);
  if (status != HACKRF_SUCCESS) {
    fprintf(stderr, "Failed to open device.\r\n");
  }
  
  uint64_t freq {13'560'000U};
  status = hackrf_set_freq( _device, freq );
  if (status != HACKRF_SUCCESS) {
    fprintf(stderr, "Failed to set frequency.\r\n");
  }
  
  uint64_t srate{8'000'000U};
  status = hackrf_set_sample_rate( _device, srate );
  if (status != HACKRF_SUCCESS) {
    fprintf(stderr, "Failed to set sample rate.\r\n");
  }
  
  status = hackrf_set_amp_enable( _device, 0 );
  if (status != HACKRF_SUCCESS) {
    fprintf(stderr, "Failed to set AMP status.\r\n");
  }
  
  uint32_t gain {32U};
  status = hackrf_set_lna_gain( _device, gain );
  if (status != HACKRF_SUCCESS) {
    fprintf(stderr, "Failed to set LNA gain.\r\n");
  }
	
  gain = 30U;
  status = hackrf_set_vga_gain( _device, gain);
  if (status != HACKRF_SUCCESS) {
    fprintf(stderr, "Failed to set VGA gain.\r\n");
  }
  
  status = hackrf_start_rx( _device, rx_callback, nullptr );
  if (status != HACKRF_SUCCESS) {
    fprintf(stderr, "Failed to start RX streaming.\r\n");
  }

  int ret_streaming {hackrf_is_streaming(_device)};
  printf("streaming %d\r\n", ret_streaming);
  
  usleep(5000);
  printf("stop!\r\n");
  
  hackrf_stop_rx(_device);

  hackrf_close(_device);
  
  hackrf_exit();
  
  return 0;
}

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

static void tx_complete_callback(hackrf_transfer* transfer, int success) {
	// If a transfer failed to complete
	if (!success) { }
	transfer->valid_length;
	transfer->buffer;
	// byte_count += transfer->valid_length;
}

int main2() {
  int result;
  int txvga_gain;
  hackrf_device *_device {};
  result = hackrf_set_txvga_gain(_device, txvga_gain);
  //result |= hackrf_enable_tx_flush(device, flush_callback, NULL);
  //result |= hackrf_set_tx_block_complete_callback(_device, tx_complete_callback);
  result |= hackrf_start_tx(_device, tx_callback, NULL);
  
  
  result = hackrf_stop_tx(_device);
  if (result != HACKRF_SUCCESS) {
	fprintf(stderr, "hackrf_stop_tx() failed: %d\r\n",result);
  }
	
  return 0;
}