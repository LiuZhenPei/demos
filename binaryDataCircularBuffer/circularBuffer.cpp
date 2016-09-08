
#include <string.h>
#include <algorithm> // for std::min
#include <iostream>

using namespace std;

class CircularBuffer {
    public:
        CircularBuffer(size_t capacity);
        ~CircularBuffer();

        size_t size() const { return size_; }
        size_t capacity() const { return capacity_; }
        // Return number of bytes written.
        size_t write(const char *data, size_t bytes);
        // Return number of bytes read.
        size_t read(char *data, size_t bytes);

    private:
        size_t beg_index_, end_index_, size_, capacity_;
        char *data_;
};

CircularBuffer::CircularBuffer(size_t capacity)
    : beg_index_(0),
    end_index_(0),
    size_(0),
    capacity_(capacity) {
    data_ = new char[capacity];
}

CircularBuffer::~CircularBuffer() {
    delete [] data_;
}

// Return bytes that actually write in, would not write if not space left.
size_t CircularBuffer::write(const char *data, size_t bytes) {
    if (bytes == 0)
        return 0;

    size_t capacity = capacity_;
    size_t bytes_to_write = std::min(bytes, capacity - size_);

    // Write in a single step
    if (bytes_to_write <= capacity - end_index_) {
        memcpy(data_ + end_index_, data, bytes_to_write);
        end_index_ += bytes_to_write;
        if (end_index_ == capacity) end_index_ = 0;
    }
    // Write in two steps
    else {
        size_t size_1 = capacity - end_index_;
        memcpy(data_ + end_index_, data, size_1);
        size_t size_2 = bytes_to_write - size_1;
        memcpy(data_, data + size_1, size_2);
        end_index_ = size_2;
    }

    size_ += bytes_to_write;
    return bytes_to_write;
}

// Return bytes that read.
size_t CircularBuffer::read(char *data, size_t bytes) {
    if (bytes == 0) return 0;

    size_t capacity = capacity_;
    size_t bytes_to_read = std::min(bytes, size_);

    // Read in a single step
    if (bytes_to_read <= capacity - beg_index_) {
        memcpy(data, data_ + beg_index_, bytes_to_read);
        beg_index_ += bytes_to_read;
        if (beg_index_ == capacity) beg_index_ = 0;
    }
    // Read in two steps
    else {
        size_t size_1 = capacity - beg_index_;
        memcpy(data, data_ + beg_index_, size_1);
        size_t size_2 = bytes_to_read - size_1;
        memcpy(data + size_1, data_, size_2);
        beg_index_ = size_2;
    }

    size_ -= bytes_to_read;
    return bytes_to_read;
}

int main() {
    char tmpBuf[1024];
    char helloBuf[] = "hello world"; // 11

    CircularBuffer buf(1024);

    cout << "capacity: " << buf.capacity() << endl;
    cout << "size: " << buf.size() << endl;

    int nwrite = buf.write(helloBuf, 11);
    cout << "nwrite: " << nwrite << endl;

    nwrite = buf.write(tmpBuf, 1024);
    cout << "nwrite: " << nwrite << endl;

    int nread = buf.read(tmpBuf, 1020);
    cout << "nread: " << nread << endl;

    nread = buf.read(tmpBuf, 100);
    cout << "nread: " << nread << endl;

    return 0;
}
