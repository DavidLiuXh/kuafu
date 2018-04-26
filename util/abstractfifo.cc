#include "stdafx.h"
#include <cstdlib>
#include <lutil/util/AbstractFifo.hxx>

#include <cassert>

using namespace LUtil;

AbstractFifo::AbstractFifo(unsigned int maxSize)
   : mSize(0),
     mMaxSize(maxSize)
{
}

AbstractFifo::~AbstractFifo()
{}

bool
AbstractFifo::empty() const
{
   boost::mutex::scoped_lock lock(mMutex); 
   return mSize == 0;
}

unsigned int
AbstractFifo::size() const
{
   boost::mutex::scoped_lock lock(mMutex); 
   return mSize;
}
