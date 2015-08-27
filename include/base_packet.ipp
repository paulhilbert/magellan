namespace magellan {

template <class Header, int MaxBodyLength>
inline
base_packet<Header, MaxBodyLength>::base_packet() : body_length_(0) {
}

template <class Header, int MaxBodyLength>
inline
base_packet<Header, MaxBodyLength>::base_packet(const base_packet<Header, MaxBodyLength>& other) {
	body_length_ = other.body_length_;
    memcpy(data_, other.data_, other.length());
}

template <class Header, int MaxBodyLength>
inline
base_packet<Header, MaxBodyLength>& base_packet<Header, MaxBodyLength>::operator =(const base_packet<Header, MaxBodyLength>& other) {
	body_length_ = other.body_length_;
    memcpy(data_, other.data_, other.length());
	return *this;
}

template <class Header, int MaxBodyLength>
inline
base_packet<Header, MaxBodyLength>::~base_packet() {
}

template <class Header, int MaxBodyLength>
inline const typename base_packet<Header, MaxBodyLength>::data_t*
base_packet<Header, MaxBodyLength>::data() const {
    return data_;
}

template <class Header, int MaxBodyLength>
inline typename base_packet<Header, MaxBodyLength>::data_t*
base_packet<Header, MaxBodyLength>::data() {
	return data_;
}

template <class Header, int MaxBodyLength>
inline typename base_packet<Header, MaxBodyLength>::length_t
base_packet<Header, MaxBodyLength>::length() const {
	return header_length + body_length_;
}

template <class Header, int MaxBodyLength>
inline const typename base_packet<Header, MaxBodyLength>::data_t*
base_packet<Header, MaxBodyLength>::body() const {
	return data_ + header_length;
}

template <class Header, int MaxBodyLength>
inline typename base_packet<Header, MaxBodyLength>::data_t*
base_packet<Header, MaxBodyLength>::body() {
	return data_ + header_length;
}

template <class Header, int MaxBodyLength>
inline typename base_packet<Header, MaxBodyLength>::length_t
base_packet<Header, MaxBodyLength>::body_length() const {
	return body_length_;
}

template <class Header, int MaxBodyLength>
inline void
base_packet<Header, MaxBodyLength>::set_body_length(length_t len) {
	body_length_ = len;
}

} // magellan
