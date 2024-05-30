#ifndef UTILITIES_STRUCT_OF_ARRAY_TPP
#define UTILITIES_STRUCT_OF_ARRAY_TPP

#include <cassert>
#include <utilities.hpp>


template <typename ObjectID, ObjectID N, typename... Args>
std::size_t utility::StructOfArray<ObjectID, N, std::tuple<Args...>>::GetObjectCount() const noexcept {
    return mObjectCount;
}

template <typename ObjectID, ObjectID N, typename... Args>
void utility::StructOfArray<ObjectID, N, std::tuple<Args...>>::Insert(ObjectID objectID, Object const& object) {
    assert(mObjectIDToMemberIndexMap.find(objectID) == mObjectIDToMemberIndexMap.end() && "Object added to same ID more than once");

    // Place new object at the end
    auto indexOfNewObject = mObjectCount++;

    // Update maps
    mObjectIDToMemberIndexMap[objectID] = indexOfNewObject;
    mMemberIndexToObjectIDMap[indexOfNewObject] = objectID;
    InsertObject(indexOfNewObject, object);
}

template <typename ObjectID, ObjectID N, typename... Args>
void utility::StructOfArray<ObjectID, N, std::tuple<Args...>>::Erase(ObjectID objectID) {
    assert(mObjectIDToMemberIndexMap.find(objectID) != mObjectIDToMemberIndexMap.end() && "Removing non-existent object");

    // Copy last object (back) into removed object's place
    auto indexOfRemovedEntry = mObjectIDToMemberIndexMap[objectID];
    auto indexOfLastEntry = --mObjectCount;
    InsertObject(indexOfRemovedEntry, indexOfLastEntry);

    // Update maps
    auto idOfLastEntry = mMemberIndexToObjectIDMap[indexOfLastEntry];
    mObjectIDToMemberIndexMap[idOfLastEntry] = indexOfRemovedEntry;
    mMemberIndexToObjectIDMap[indexOfRemovedEntry] = idOfLastEntry;

    mObjectIDToMemberIndexMap.erase(objectID);
    mMemberIndexToObjectIDMap.erase(indexOfLastEntry);        
}

template <typename ObjectID, ObjectID N, typename... Args>
template <std::size_t I>
utility::StructOfArray<ObjectID, N, std::tuple<Args...>>::template Member<I>& utility::StructOfArray<ObjectID, N, std::tuple<Args...>>::Get(ObjectID objectID) {
    static_assert(I < GetMemberCount());
    assert(mObjectIDToMemberIndexMap.find(objectID) != mObjectIDToMemberIndexMap.end() && "Retrieving non-existent object");
    return std::get<I>(mBuffer)[mObjectIDToMemberIndexMap[objectID]];
}

template <typename ObjectID, ObjectID N, typename... Args>
template <std::size_t I>
void utility::StructOfArray<ObjectID, N, std::tuple<Args...>>::InsertObject(ObjectID objectID, Object const& object) {
    if constexpr(I < GetMemberCount()) {
        auto& memberArray = std::get<I>(mBuffer);
        memberArray[objectID] = std::get<I>(object);

        InsertObject<I + 1>(objectID, object);
    }
}

template <typename ObjectID, ObjectID N, typename... Args>
template <std::size_t I>
void utility::StructOfArray<ObjectID, N, std::tuple<Args...>>::InsertObject(ObjectID objectID, ObjectID srcObjectID) {
    if constexpr(I < GetMemberCount()) {
        auto& memberArray = std::get<I>(mBuffer);
        memberArray[objectID] = memberArray[srcObjectID];

        InsertObject<I + 1>(objectID, srcObjectID);
    }
}



#endif