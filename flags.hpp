/********************************************************
 * Wrapper class and helpers for flag manipulation
 *
 * Author: Sylvain Leclercq <maisbiensurqueoui@gmail.com>
 * Created: 		4/14/2014
 * Last modified: 	5/14/2014
 * Version: 0.0.1
 *******************************************************/

#ifndef DPSG_FLAGS_HPP
#define DPSG_FLAGS_HPP

#include <iterator>

#define DPSG_PP_NARG(...) \
	DPSG_PP_NARG_(__VA_ARGS__, DPSG_PP_RSEQ_N())

#define DPSG_PP_NARG_(...) \
	DPSG_PP_ARG_N(__VA_ARGS__)

#define DPSG_PP_ARG_N( \
		_1, _2, _3, _4, _5, _6, _7, _8,	N, ...) N

#define DPSG_PP_RSEQ_N()	\
	8, 7, 6, 5, 4, 3, 2, 1

#define DPSG_PP_EXPAND(count, ...)	\
	DPSG_PP_EXPAND_##count (__VA_ARGS__)

#define DPSG_PP_EXPAND_1(a) a = 1
#define DPSG_PP_EXPAND_2(a, b) DPSG_PP_EXPAND_1(a), b = 1 << 1
#define DPSG_PP_EXPAND_3(a, b, c) DPSG_PP_EXPAND_2(a, b), c = 1 << 2
#define DPSG_PP_EXPAND_4(a, b, c, d) DPSG_PP_EXPAND_3(a, b, c), d = 1 << 3
#define DPSG_PP_EXPAND_5(a, b, c, d, e) DPSG_PP_EXPAND_4(a, b, c, d), e = 1 << 4
#define DPSG_PP_EXPAND_6(a, b, c, d, e, f) DPSG_PP_EXPAND_5(a, b, c, d, e), f = 1 << 5
#define DPSG_PP_EXPAND_7(a, b, c, d, e, f, g) DPSG_PP_EXPAND_6(a, b, c, d, e, f), g = 1 << 6
#define DPSG_PP_EXPAND_8(a, b, c, d, e, f, g, h) DPSG_PP_EXPAND_7(a, b, c, d, e, f, g), h = 1 << 7

#define DPSG_DECLARE_FLAG_(TYPE, NAME, COUNT, ...) 					\
	class NAME														\
		: public dpsg::detail::BaseFlag<TYPE, COUNT> {				\
		typedef BaseFlag<TYPE, COUNT> Base;							\
		public:														\
		enum {DPSG_PP_EXPAND(COUNT, __VA_ARGS__)}; 					\
		NAME() : Base(Base::Count) {}								\
		NAME(TYPE i) : Base(i) {} 									\
	};

#define DPSG_DECLARE_FLAG(type, name, ...) DPSG_DECLARE_FLAG_(type, name, DPSG_PP_NARG(__VA_ARGS__), __VA_ARGS__)

namespace dpsg { namespace detail
{

template<class T, char C>
class BaseFlag
{
	public:
		typedef T type;
	private:
		typedef BaseFlag<type, C>& this_t;
		typedef char count_t;
	public:
		constexpr static count_t Count = C;
		class proxy;
		
		class iterator : public std::iterator<std::random_access_iterator_tag, proxy>
		{
			typedef std::iterator<std::random_access_iterator_tag, proxy> traits;
			typedef typename traits::difference_type difference_type;
			public:
				iterator() = delete;
				iterator(this_t r, count_t p = 0) : _ref(r), _pos(p), _hack(r, p) {bound();}
				inline iterator& operator++() {incr(); return *this;}
				inline iterator& operator++(int) 
					{iterator tmp = *this; incr(); return tmp;}
				inline bool operator==(const iterator& it) 
					{return (_pos == it._pos) && (_ref == it._ref);}
				inline bool operator!=(const iterator& it) {return !(*this == it);}
				inline proxy operator*() {return proxy(_ref, _pos);}
				inline proxy* operator->() {_hack._pos = 1 << _pos; return &_hack;}

			private:
				count_t _pos;
				this_t _ref;
				mutable proxy _hack; 
				inline void incr(difference_type offset = 1) {_pos += offset; bound();}
				inline void bound() 
				{
					if(_pos < 0) 
						_pos = 0; 
					else if(_pos > _ref.Count) 
						_pos = Count;
				}
		};

		class proxy
		{
			public: 
				proxy(this_t r, count_t p) : _ref(r), _pos(1 << p) {}
				proxy& operator=(bool b) { b ? set() : unset(); return *this;}
				operator bool() const { return is_set(); }
				void set() { _ref.set(_pos); } 
				void unset() { _ref.unset(_pos); }
				void toggle() { _ref.toggle(_pos); }
				bool is_set() const { return _ref.is_set(_pos); }
			private:
				friend class iterator;
				this_t _ref;
				type _pos;	
		};

		constexpr inline operator type() {return _value;}
		inline bool is_set(type v) const {return _value & v;}
		inline void set(type v) {_value |= v;}
		inline void toggle(type v) {_value ^= v;}
		inline void unset(type v) 
		{
			_value =~ (~_value | v);
		}
		iterator begin() {return iterator(*this, 0);}
		iterator end() {return iterator(*this, Count);}
		//const_iterator begin() const { return const_iterator(*this, 0);}
		//const_iterator begin() const { return const_iterator(*this, Count);}

	protected:
		BaseFlag(type i) : _value(i) {} 
		BaseFlag() = default;
	
	private: 
		T _value;
};

}}

#endif
