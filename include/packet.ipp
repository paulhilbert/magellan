#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace magellan {

template <class Header, int MaxBodyLength>
inline
packet<Header, MaxBodyLength>::packet() : base_packet<Header, MaxBodyLength>() {
}

template <class Header, int MaxBodyLength>
inline
packet<Header, MaxBodyLength>::packet(const packet<Header, MaxBodyLength>& other) {
	header_      = other.header_;
	this->body_length_ = other.body_length_;
    memcpy(this->data_, other.data_, other.length());
}

template <class Header, int MaxBodyLength>
inline packet<Header, MaxBodyLength>&
packet<Header, MaxBodyLength>::operator =(const packet<Header, MaxBodyLength>& other) {
	header_      = other.header_;
	this->body_length_ = other.body_length_;
    memcpy(this->data_, other.data_, other.length());
	return *this;
}

template <class Header, int MaxBodyLength>
inline
packet<Header, MaxBodyLength>::~packet() {
}

template <class Header, int MaxBodyLength>
inline const Header&
packet<Header, MaxBodyLength>::header() const {
	return header_;
}

template <class Header, int MaxBodyLength>
inline Header&
packet<Header, MaxBodyLength>::header() {
	return header_;
}

template <class Header, int MaxBodyLength>
inline void
packet<Header, MaxBodyLength>::encode() {
    memcpy(this->data_, (const char*) &this->body_length_, sizeof(length_t));
    memcpy(this->data_ + sizeof(length_t), (const char*) &header_, sizeof(header_));
}

template <class Header, int MaxBodyLength>
inline void
packet<Header, MaxBodyLength>::decode() {
    memcpy((char*)&this->body_length_, this->data_, sizeof(length_t));
    memcpy((char*)&this->header_, this->data_ + sizeof(length_t), sizeof(Header));
}

template <int MaxBodyLength>
inline
packet<void, MaxBodyLength>::packet() : base_packet<void, MaxBodyLength>() {
}

template <int MaxBodyLength>
inline
packet<void, MaxBodyLength>::~packet() {
}

template <int MaxBodyLength>
inline void
packet<void, MaxBodyLength>::encode() {
    memcpy(this->data_, (const char*) &this->body_length_, sizeof(length_t));
}

template <int MaxBodyLength>
inline void
packet<void, MaxBodyLength>::decode() {
    memcpy((char*)&this->body_length_, this->data_, sizeof(length_t));
}

} // magellan



