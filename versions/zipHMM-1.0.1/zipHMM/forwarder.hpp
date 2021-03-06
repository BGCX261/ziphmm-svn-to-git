#ifndef FORWARDER_HPP
#define FORWARDER_HPP

#include "matrix.hpp"
#include "PThreadProcessingDevice.hpp"
#include "performance_description.hpp"
#include "hmm_utils.hpp"

#include <utility>
#include <algorithm>
#include <map>
#include <vector>
#include <fstream>
#include <cstdlib>

namespace zipHMM {

  class SimpleStopForwarder;

  class Forwarder {

  protected:
    size_t orig_seq_length;
    size_t orig_alphabet_size;
    std::map<unsigned, s_pair> symbol2pair;
    std::map<size_t, size_t> nStates2alphabet_size;
    std::map<size_t, std::vector<unsigned> > nStates2seq;
    
  public:
    friend class SimpleStopForwarder;

    Forwarder() { }
    ~Forwarder() { }

    size_t get_orig_seq_length() const { return orig_seq_length; }
    size_t get_orig_alphabet_size() const { return orig_alphabet_size; }
    size_t get_seq_length(size_t no_states) const;
    size_t get_alphabet_size(size_t no_states) const;
    s_pair get_pair(unsigned symbol) const;

    double forward(const Matrix &pi, const Matrix &A, const Matrix &B) const;
    double pthread_forward(const Matrix &pi, const Matrix &A, const Matrix &B, const std::string &device_filename = DEFAULT_DEVICE_FILENAME) const;
    double pthread_forward(const Matrix &pi, const Matrix &A, const Matrix &B, const DeviceDescriptor &device_descriptor) const;

    double pthread_forward_par_stage1(const Matrix &pi, const Matrix &A, const Matrix &B, const std::string &device_filename = DEFAULT_DEVICE_FILENAME) const;
    double pthread_forward_par_stage1(const Matrix &pi, const Matrix &A, const Matrix &B, const DeviceDescriptor &device_descriptor) const;

    void write_to_directory(const std::string &directory) const;

    void read_seq(const std::string &seq_filename, const size_t alphabet_size, const size_t min_no_eval = 1);
    void read_seq(const std::string &seq_filename, const size_t alphabet_size, std::vector<size_t> nStatesSave, const size_t min_no_eval = 1);
    void read_seq(const std::string &seq_filename, const size_t alphabet_size, const size_t no_states, const size_t min_no_eval); // convenient

    void read_from_directory(const std::string &dirname);
    void read_from_directory(const std::string &directory, const size_t no_states);

  private:
    void compute_symbol2scale_and_symbol2matrix(Matrix *symbol2matrix, double *symbol2scale, const Matrix &A, const Matrix &B, size_t alphabet_size) const;
    void pthread_compute_symbol2scale_and_symbol2matrix(double *symbol2scale, Matrix *symbol2matrix, const Matrix &A, const Matrix &B, const size_t alphabet_size, const std::vector<ProcessingDevice*> &devices) const;
    
    void write_seqs(const std::string nstates2seq_absolute_dir_name) const;
    void write_seq(const std::string &seq_filename, size_t no_states) const;
    void write_seq(std::ofstream &seq_stream, size_t no_states) const;

    void write_data_structure(const std::string &data_structure_filename) const;
    void write_data_structure(std::ofstream &data_structure_stream) const;

    void read_data_structure_from_directory(const std::string &directory);
    void read_data_structure_from_stream(std::ifstream &data_structure_stream);
    
    void read_seqs_from_directory(const std::string &directory);
    void read_seq_from_directory(const std::string &directory, size_t no_states);
    void read_seq_from_directory(std::ifstream &seq_stream, size_t no_states);
  };
  
}

#endif
