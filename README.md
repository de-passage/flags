#### Flags

Flags are awesome, but bitwise arithmetic is annoying. Hence the wrapper. 

Use like this : 

~~~ cpp
#include <cassert>
#include "flags.hpp"

DPSG_DECLARE_FLAG(char, MyFlags, option_1, option_2, option_3)
//                 ^       ^        ^         ^          ^
//                 |       |         \        |         /
//    underlying type     name       options (max 8 atm.)

int main() {
    MyFlags flags = MyFlags::option_1 | MyFlags::option_2;

    assert( flags.is_set(MyFlags::option_1 | MyFlags::option_2) );
    assert( !flags[MyFlags::option_3] );

    flags[MyFlags::option_1] = false;
	assert( !flags[MyFlags::option_1].is_set() );

	flags.toggle(MyFlags::option_1) ;
	assert( flags[MyFlags::option_1 | MyFlags::option_2] );
    assert( !flags[MyFlags::option_3] );
	
    flags.unset(MyFlags::option_1 | MyFlags::option_2);
    assert( flags.empty() );

    return 0;
}	
~~~

In a nutshell you can reference your flags individually or by groups with the index operator, or call member functions directly. 
It is also possible to iterate through the flags just like you would with an array. 
Note though that since there is no such thing as a single bit in C++,
dereferencing iterators or calling the index operator actually returns a proxy
to the bit (or bits if you feed several options to []).

#### A quick cheatsheet 

##### Macro:

    DPSG_DECLARE_FLAG(type, name, options...)
This macro simply generate a class [name] and populate with an
enumeration (class enum of undelying type [type]) of [options], as well as a
few functions. The underlying type must be large enough to handle the number
of options you define thereafter or you'll get a compilation error (in the
best case). Since the class does not support over 8 options yet it doesn't matter,
just give **char**.

##### Functions:

These functions will be defined by DPSG_DECLARE_FLAG as members of the flag class. The type "type" in the following refers to the type used to declare the strong enum holding the options.

    bool is_set(type options) const
Returns true if the option(s) sent as arguments are all
set to true.


    void set(type options)
Set the option(s) given to true.


    void unset(type options)
Set the option(s) given to false.


    void toggle(type options)
Set the given options to true if they were false and false if they were true.


    proxy operator[type options]
    const_proxy operator[type options] const
Returns a proxy to the option(s) given as arguments. A proxy can basically
call set(), unset(), toggle() and is_set() on the bits they refer to. They
also implicitely convert to **bool**.


	iterator begin()
	iterator end()
	const_iterator begin() const
	const_iterator end() const
Do exactly what you would expect them to do. 


    bool empty() const 
Returns true if all the flags are set to false. True otherwise.


	template<T> T to() const
Converts the flags to a T. For compatibility.

#### A word

This is a work in progress, nothing is set in stone for now. Also note that I
code this thing for the fun of playing with bitwise operators, there are
probably a lot of better options out there.

