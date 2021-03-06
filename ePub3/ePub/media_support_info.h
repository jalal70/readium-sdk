//
//  media_support_info.h
//  ePub3
//
//  Created by Jim Dovey on 2013-02-04.
//  Copyright (c) 2013 The Readium Foundation. All rights reserved.
//

#ifndef __ePub3__media_support_info__
#define __ePub3__media_support_info__

#include "epub3.h"
#include <map>
#include <list>

EPUB3_BEGIN_NAMESPACE

class Package;
class ManifestItem;

class MediaSupportInfo
{
public:
    ///
    /// A list of manifest items.
    typedef std::list<const ManifestItem*>      ManifestItemList;
    
    ///
    /// Possible types of support provided.
    enum class SupportType : uint8_t
    {
        Unsupported             = 0,    ///< The media type is not supported.
        IntrinsicSupport        = 1,    ///< The rendering engine undersands this media type with no outside help.
        SupportedWithHandler    = 2     ///< The publication provides a DHTML handler for this media type.
    };
    
public:
    ///
    /// The default constructor creates an object with no information.
                    MediaSupportInfo() : MediaSupportInfo("", SupportType::Unsupported) {}
    ///
    /// A simple constructor, normally used to define intrinsic support.
                    MediaSupportInfo(const string& mediaType, bool supported=true);
    ///
    /// The real constructor takes a specific SupportType value.
                    MediaSupportInfo(const string& mediaType, SupportType support);
    ///
    /// Copy constructor.
                    MediaSupportInfo(const MediaSupportInfo& o);
    ///
    /// C++11 'move' constructor.
                    MediaSupportInfo(MediaSupportInfo&& o);
    ///
    /// Destructor.
    virtual         ~MediaSupportInfo() {}
    
    ///
    /// Copy assignment operator.
    MediaSupportInfo&   operator=(const MediaSupportInfo& o);
    ///
    /// C++11 move assignment operator.
    MediaSupportInfo&   operator=(MediaSupportInfo&& o);
    
    /**
     @defgroup Accessors Accessors
     @{
     */
    
    /**
     Returns the media type represented by this object.
     */
    const string    MediaType()             const   { return _mediaType; }
    
    /**
     Returns the type of support as a SupportType code.
     */
    SupportType     Support()               const   { return _support; }
    
    /**
     Queries whether a type is supported intrinsically by the rendering engine.
     */
    virtual bool    HasIntrinsicSupport()   const;
    
    /**
     Queries whether a type is supported through the use of a special handler.
     */
    virtual bool    RequiresMediaHandler()  const;
    
    /** @} */
    
    /**
     @defgroup Setters Setters
     @{
     */
    
    /**
     Sets the media type and its support state.
     
     This is here as a convenience to anyone working with C++ containers.
     @param mediaType The media type.
     @param support The type of support provided.
     */
    virtual void    SetTypeAndSupport(const string& mediaType, SupportType support);
    
    /** @} */
    
    /**
     Constructs a list of manifest items with this media type.
     @param pkg The package whose manifest to look through.
     @result A vector containing manifest items matching the receiver's media type.
     */
    const ManifestItemList  MatchingManifestItems(const Package* pkg) const;
    
protected:
    string              _mediaType;     ///< The media type to which this object refers.
    SupportType         _support;       ///< The type of support available for this type.
    
};

EPUB3_END_NAMESPACE

#endif /* defined(__ePub3__media_support_info__) */
