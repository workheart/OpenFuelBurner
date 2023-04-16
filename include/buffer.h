

/*
 this is the cicular buffer class 
 initialize em like
  CircularBuffer buffer1; // the first temp buffer the name are up to you
  CircularBuffer buffer2; // the scnd temp buffer
        but! initializ afer you #includet the class 
        outside the loop  setup or defore
everytime you read temp 
  buffer1.push(tempanal);
  buffer2.push(Board_Voltage); 
      use the push funktion 

to read the medium average:
    smothtem1       = buffer1.filteredTemp();
    smothtemp2      = buffer2.filteredTemp();
    returns float
*/

#define BUFFER_SIZE 25
class CircularBuffer {
public:
  CircularBuffer() : start_(0), end_(0) { //initial load buffer with 0os
    for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer_[i] = 0;}
    }

  void push(float temperature) {// Serial.print("end_: ");Serial.println(end_);
    buffer_[end_] = temperature;
    end_ = (end_ + 1) % BUFFER_SIZE;
    if (end_ == start_) {
      // buffer is full, so move the start pointer forward
      start_ = (start_ + 1) % BUFFER_SIZE;
    }
  }

  float filteredTemp() const {
    // compute the filtered temperature value
float sum = 0;
      // for (int i = 0; i < BUFFER_SIZE; i++) {                   // more redable
  for (int i = start_; i != end_; i = (i + 1) % BUFFER_SIZE) {    // more elegant

  sum = sum + buffer_[i];
}
    return (sum / BUFFER_SIZE) ;
  }

private:
  float buffer_[BUFFER_SIZE];
  int start_;
  int end_;
};