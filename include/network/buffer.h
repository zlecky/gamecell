#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <string>
#include <vector>

namespace TinyNet {
    class Slice {
    public:
        Slice() : pb_("") { pe_ = pb_; }
        Slice(const char *b, const char *e) : pb_(b), pe_(e) {}
        Slice(const char *d, size_t n) : pb_(d), pe_(d + n) {}
        Slice(const std::string &s) : pb_(s.data()), pe_(s.data() + s.size()) {}
        Slice(const char *s) : pb_(s), pe_(s + strlen(s)) {}

        const char *data() const { return pb_; }
        const char *begin() const { return pb_; }
        const char *end() const { return pe_; }
        char front() { return *pb_; }
        char back() { return pe_[-1]; }
        size_t size() const { return pe_ - pb_; }
        void resize(size_t sz) { pe_ = pb_ + sz; }
        inline bool empty() const { return pe_ == pb_; }
        void clear() { pe_ = pb_ = ""; }

        // return the eated data
        Slice eatWord();
        Slice eatLine();
        Slice eat(int sz) {
            Slice s(pb_, sz);
            pb_ += sz;
            return s;
        }
        Slice sub(int boff, int eoff = 0) const {
            Slice s(*this);
            s.pb_ += boff;
            s.pe_ += eoff;
            return s;
        }
        Slice &trimSpace();

        inline char operator[](size_t n) const { return pb_[n]; }

        std::string toString() const { return std::string(pb_, pe_); }
        // Three-way comparison.  Returns value:
        int compare(const Slice &b) const;

        // Return true if "x" is a prefix of "*this"
        bool starts_with(const Slice &x) const { return (size() >= x.size() && memcmp(pb_, x.pb_, x.size()) == 0); }

        bool end_with(const Slice &x) const { return (size() >= x.size() && memcmp(pe_ - x.size(), x.pb_, x.size()) == 0); }
        operator std::string() const { return std::string(pb_, pe_); }
        std::vector<Slice> split(char ch) const;

    private:
        const char *pb_;
        const char *pe_;
    };

    struct Buffer {
        Buffer() : buf_(NULL), b_(0), e_(0), cap_(0), exp_(512) {}
        ~Buffer() { delete[] buf_; }
        void clear() {
            delete[] buf_;
            buf_ = NULL;
            cap_ = 0;
            b_ = e_ = 0;
        }
        size_t size() const { return e_ - b_; }
        bool empty() const { return e_ == b_; }
        char *data() const { return buf_ + b_; }
        char *begin() const { return buf_ + b_; }
        char *end() const { return buf_ + e_; }
        char *makeRoom(size_t len) {
            if (e_ + len <= cap_) {
            } else if (size() + len < cap_ / 2) {
                moveHead();
            } else {
                expand(len);
            }
            return end();
        }
        void makeRoom() {
            if (space() < exp_)
                expand(0);
        }
        size_t space() const { return cap_ - e_; }
        void addSize(size_t len) { e_ += len; }
        char *allocRoom(size_t len) {
            char *p = makeRoom(len);
            addSize(len);
            return p;
        }
        Buffer &append(const char *p, size_t len) {
            memcpy(allocRoom(len), p, len);
            return *this;
        }
        Buffer &append(Slice slice) { return append(slice.data(), slice.size()); }
        Buffer &append(const char *p) { return append(p, strlen(p)); }
        template <class T>
        Buffer &appendValue(const T &v) {
            append((const char *) &v, sizeof v);
            return *this;
        }
        Buffer &consume(size_t len) {
            b_ += len;
            if (size() == 0)
                clear();
            return *this;
        }
        Buffer &absorb(Buffer &buf) {
            if (&buf != this) {
                if (size() == 0) {
                    char b[sizeof buf];
                    memcpy(b, this, sizeof b);
                    memcpy(this, &buf, sizeof b);
                    memcpy(&buf, b, sizeof b);
                    std::swap(exp_, buf.exp_);  // keep the origin exp_
                } else {
                    append(buf.begin(), buf.size());
                    buf.clear();
                }
            }
            return *this;
        }
        void setSuggestSize(size_t sz) { exp_ = sz; }
        Buffer(const Buffer &b) { copyFrom(b); }
        Buffer &operator=(const Buffer &b) {
            if (this == &b)
                return *this;
            delete[] buf_;
            buf_ = NULL;
            copyFrom(b);
            return *this;
        }
        operator Slice() { return Slice(data(), size()); }

    private:
        char *buf_;
        size_t b_, e_, cap_, exp_;
        void moveHead() {
            std::copy(begin(), end(), buf_);
            e_ -= b_;
            b_ = 0;
        }
        void expand(size_t len) {
            size_t ncap = std::max(exp_, std::max(2 * cap_, size() + len));
            char *p = new char[ncap];
            std::copy(begin(), end(), p);
            e_ -= b_;
            b_ = 0;
            delete[] buf_;
            buf_ = p;
            cap_ = ncap;
        }
        void copyFrom(const Buffer &b) {
            memcpy(this, &b, sizeof b);
            if (b.buf_) {
                buf_ = new char[cap_];
                memcpy(data(), b.begin(), b.size());
            }
        }
    };

    inline Slice Slice::eatWord() {
        const char *b = pb_;
        while (b < pe_ && isspace(*b)) {
            b++;
        }
        const char *e = b;
        while (e < pe_ && !isspace(*e)) {
            e++;
        }
        pb_ = e;
        return Slice(b, e - b);
    }

    inline Slice Slice::eatLine() {
        const char *p = pb_;
        while (pb_ < pe_ && *pb_ != '\n' && *pb_ != '\r') {
            pb_++;
        }
        return Slice(p, pb_ - p);
    }

    inline Slice &Slice::trimSpace() {
        while (pb_ < pe_ && isspace(*pb_))
            pb_++;
        while (pb_ < pe_ && isspace(pe_[-1]))
            pe_--;
        return *this;
    }

    inline bool operator<(const Slice &x, const Slice &y) {
        return x.compare(y) < 0;
    }

    inline bool operator==(const Slice &x, const Slice &y) {
        return ((x.size() == y.size()) && (memcmp(x.data(), y.data(), x.size()) == 0));
    }

    inline bool operator!=(const Slice &x, const Slice &y) {
        return !(x == y);
    }

    inline int Slice::compare(const Slice &b) const {
        size_t sz = size(), bsz = b.size();
        const int min_len = (sz < bsz) ? sz : bsz;
        int r = memcmp(pb_, b.pb_, min_len);
        if (r == 0) {
            if (sz < bsz)
                r = -1;
            else if (sz > bsz)
                r = +1;
        }
        return r;
    }

    inline std::vector<Slice> Slice::split(char ch) const {
        std::vector<Slice> r;
        const char *pb = pb_;
        for (const char *p = pb_; p < pe_; p++) {
            if (*p == ch) {
                r.push_back(Slice(pb, p));
                pb = p + 1;
            }
        }
        if (pe_ != pb_)
            r.push_back(Slice(pb, pe_));
        return r;
    }
}

#endif//__BUFFER_H__