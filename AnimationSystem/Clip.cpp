#include "Clip.h"

Clip::Clip()
{
    mName = "No name given";
    mStartTime = 0.0f;
    mEndTime = 0.0f;
    mLooping = true;
}

float Clip::AdjustTimeToFitRange(float inTime)
{
    if (mLooping) {
        float duration = mEndTime - mStartTime;
        if (duration <= 0) { 0.0f; }
        inTime = fmodf(inTime - mStartTime,
            mEndTime - mStartTime);
        if (inTime < 0.0f) {
            inTime += mEndTime - mStartTime;
        }
        inTime = inTime + mStartTime;
    }
    else {
        if (inTime < mStartTime) {
            inTime = mStartTime;
        }
        if (inTime > mEndTime) {
            inTime = mEndTime;
        }
    }
    return inTime;
}

unsigned int Clip::Size()
{
    return (unsigned int)mTracks.size();
}

float Clip::Sample(Pose& outPose, float inTime)
{
    if (GetDuration() == 0.0f) {
        return 0.0f;
    }
    inTime = AdjustTimeToFitRange(inTime);
    unsigned int size = mTracks.size();
    for (unsigned int i = 0; i < size; ++i) {
        unsigned int j = mTracks[i].GetId(); // Joint
        Transform local = outPose.GetLocalTransform(j);
        Transform animated = mTracks[i].Sample(
            local, inTime, mLooping);
        outPose.SetLocalTransform(j, animated);
    }
    return inTime;
}

TransformTrack& Clip::operator[](unsigned int joint)
{
    for (int i = 0, s = mTracks.size(); i < s; ++i) {
        if (mTracks[i].GetId() == joint) {
            return mTracks[i];
        }
    }
    mTracks.push_back(TransformTrack());
    mTracks[mTracks.size() - 1].SetId(joint);
    return mTracks[mTracks.size() - 1];
}

void Clip::RecalculateDuration()
{
    mStartTime = 0.0f;
    mEndTime = 0.0f;
    bool startSet = false;
    bool endSet = false;
    unsigned int tracksSize = mTracks.size();
    for (unsigned int i = 0; i < tracksSize; ++i) {
        if (mTracks[i].IsValid()) {
            float startTime = mTracks[i].GetStartTime();
            float endTime = mTracks[i].GetEndTime();
            if (startTime < mStartTime || !startSet) {
                mStartTime = startTime;
                startSet = true;
            }
            if (endTime > mEndTime || !endSet) {
                mEndTime = endTime;
                endSet = true;
            }
        }
    }
}

std::string& Clip::GetName() {
    return mName;
}
unsigned int Clip::GetIdAtIndex(unsigned int index) {
    return mTracks[index].GetId();
}
unsigned int Clip::Size() {
    return (unsigned int)mTracks.size();
}
float Clip::GetDuration() {
    return mEndTime - mStartTime;
}
float Clip::GetStartTime() {
    return mStartTime;
}
float Clip::GetEndTime() {
    return mEndTime;
}
bool Clip::GetLooping() {
    return mLooping;
}

void Clip::SetLooping(bool inLooping)
{
    mLooping = inLooping;
}

unsigned int Clip::GetIdAtIndex(unsigned int index)
{
    return mTracks[index].GetId();
}

void Clip::SetIdAtIndex(unsigned int index, unsigned int id)
{
    return mTracks[index].SetId(id);
}
