/********************************************************
 * Wrapper class and helpers for flag manipulation
 *
 * Author: Sylvain Leclercq <maisbiensurqueoui@gmail.com>
 * Created: 		4/14/2014
 * Last modified: 	5/14/2014
 *******************************************************/

#ifndef DPSG_FLAGS_HPP
#define DPSG_FLAGS_HPP


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
		: dpsg::detail::BaseFlag<TYPE, COUNT> { 					\
		typedef BaseFlag<TYPE, COUNT> Base;							\
		public:														\
		enum {DPSG_PP_EXPAND(COUNT, __VA_ARGS__)}; 					\
		NAME() : Base(Base::Count) {}								\
		NAME(TYPE i) : Base(i) {} 									\
			using Base::is_set;										\
			using Base::set;										\
			using Base::unset;										\
			using Base::toggle;										\
			using Base::Count;										\
			using Base::type;										\
	};

#define DPSG_DECLARE_FLAG(type, name, ...) DPSG_DECLARE_FLAG_(type, name, DPSG_PP_NARG(__VA_ARGS__), __VA_ARGS__)

namespace dpsg { namespace detail
{

template<class T, T C>
class BaseFlag
{
	public:
		typedef T type;
		constexpr static T Count = C;
		constexpr inline operator type() {return _value;}
		inline bool is_set(type v) const {return _value & v;}
		inline void set(type v) {_value |= v;}
		inline void unset(type v) {_value ^= v;}
		inline void toggle(type v) {is_set(v) ? unset(v) : set(v);}

	protected:
		BaseFlag(type i) : _value(i) {} 
		BaseFlag() = default;
	
	private: 
		T _value;
};

}}

#endif
