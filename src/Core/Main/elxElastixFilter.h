/*=========================================================================
 *
 *  Copyright UMC Utrecht and contributors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef elxElastixFilter_h
#define elxElastixFilter_h

#include "itkImageSource.h"

#include "elxElastixMain.h"
#include "elxParameterObject.h"
#include "elxPixelType.h"

/**
 * \class ElastixFilter
 * \brief Elastix registration library exposed as an ITK filter.
 */

namespace elastix
{

template< typename TFixedImage, typename TMovingImage >
class ElastixFilter : public itk::ImageSource< TFixedImage >
{
public:

  /** Standard ITK typedefs. */
  typedef ElastixFilter                   Self;
  typedef itk::ImageSource< TFixedImage > Superclass; 
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( Self, itk::ImageSource );

  /** Typedefs. */
  typedef elastix::ElastixMain                                ElastixMainType;
  typedef ElastixMainType::Pointer                            ElastixMainPointer;
  typedef std::vector< ElastixMainPointer >                   ElastixMainVectorType;
  typedef ElastixMainType::ObjectPointer                      ElastixMainObjectPointer;

  typedef ElastixMainType::FlatDirectionCosinesType           FlatDirectionCosinesType;

  typedef ElastixMainType::ArgumentMapType                    ArgumentMapType;
  typedef ArgumentMapType::value_type                         ArgumentMapEntryType;

  typedef ElastixMainType::DataObjectContainerType            DataObjectContainerType;
  typedef ElastixMainType::DataObjectContainerPointer         DataObjectContainerPointer;
  typedef DataObjectContainerType::Iterator                   DataObjectContainerIterator;
  typedef itk::ProcessObject::DataObjectIdentifierType        DataObjectIdentifierType;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType  DataObjectPointerArraySizeType;
  typedef itk::ProcessObject::NameArray                       InputNameArrayType;

  typedef ParameterObject                                     ParameterObjectType;
  typedef ParameterObjectType::ParameterMapType               ParameterMapType;
  typedef ParameterObjectType::ParameterMapVectorType         ParameterMapVectorType;
  typedef ParameterObjectType::ParameterValueVectorType       ParameterValueVectorType;
  typedef ParameterObjectType::Pointer                        ParameterObjectPointer;
  typedef ParameterObjectType::ConstPointer                   ParameterObjectConstPointer;

  typedef typename TFixedImage::Pointer                       FixedImagePointer;
  typedef typename TFixedImage::ConstPointer                  FixedImageConstPointer;
  typedef typename TMovingImage::Pointer                      MovingImagePointer;
  typedef typename TMovingImage::ConstPointer                 MovingImageConstPointer;

  itkStaticConstMacro( FixedImageDimension, unsigned int, TFixedImage::ImageDimension );
  itkStaticConstMacro( MovingImageDimension, unsigned int, TMovingImage::ImageDimension );

  typedef itk::Image< unsigned char, FixedImageDimension >    FixedMaskType;
  typedef typename FixedMaskType::Pointer                     FixedMaskPointer;
  typedef typename FixedMaskType::Pointer                     FixedMaskConstPointer;
  typedef itk::Image< unsigned char, MovingImageDimension >   MovingMaskType;
  typedef typename MovingMaskType::Pointer                    MovingMaskPointer;
  typedef typename MovingMaskType::Pointer                    MovingMaskConstPointer;

  // MS: \todo: see if you can use SetConstObjectMacro's and GetConstObjectMacro's below.
  //            have a look at the signatures of the itk::ImageToImageMetric functions and copy them if possible.
  // KM: Not possible with current input design as inputs are not member variables.

  /** Set/Get/Add/NumberOf fixed images. */
  void SetFixedImage( FixedImagePointer fixedImage );
  FixedImageConstPointer GetFixedImage( void ) const;
  FixedImageConstPointer GetFixedImage( const unsigned int index ) const;
  void AddFixedImage( FixedImagePointer fixedImage );
  unsigned int GetNumberOfFixedImages( void ) const;

  /** Set/Get/Add/NumberOf moving images. */
  void SetMovingImage( MovingImagePointer movingImages );
  MovingImageConstPointer GetMovingImage( void ) const;
  MovingImageConstPointer GetMovingImage( const unsigned int index ) const;
  void AddMovingImage( MovingImagePointer movingImage );
  unsigned int GetNumberOfMovingImages( void ) const;

  /** Set/Get/Add/Remove/NumberOf fixed masks. */
  void SetFixedMask( FixedMaskConstPointer fixedMask );
  FixedMaskConstPointer GetFixedMask( void ) const;
  FixedMaskConstPointer GetFixedMask( const unsigned int index ) const;
  void AddFixedMask( FixedMaskConstPointer fixedMask );
  void RemoveFixedMask( void );
  unsigned int GetNumberOfFixedMasks( void ) const;

  /** Set/Get/Add/Remove/NumberOf moving masks. */
  void SetMovingMask( MovingMaskConstPointer movingMask );
  MovingMaskConstPointer GetMovingMask( void ) const;
  MovingMaskConstPointer GetMovingMask( const unsigned int index ) const;
  void AddMovingMask( MovingMaskConstPointer movingMask );
  void RemoveMovingMask( void );
  unsigned int GetNumberOfMovingMasks( void ) const;

  /** Set/Get parameter object.*/
  itkSetInputMacro( ParameterObject, ParameterObject );
  ParameterObject* GetParameterObject( void );
  const ParameterObject* GetParameterObject( void ) const;
 
  /** Get transform parameter object.*/
  ParameterObject* GetTransformParameterObject( void );
  const ParameterObject* GetTransformParameterObject( void ) const;

  /** Set/Get/Remove initial transform parameter filename. */
  itkSetStringMacro( InitialTransformParameterFileName );
  itkGetConstReferenceMacro( InitialTransformParameterFileName, std::string );
  void RemoveInitialTransformParameterFileName( void ) { this->SetInitialTransformParameterFileName( "" ); };

  /** Set/Get/Remove fixed point set filename. */
  itkSetStringMacro( FixedPointSetFileName );
  itkGetConstReferenceMacro( FixedPointSetFileName, std::string );
  void RemoveFixedPointSetFileName( void ) { this->SetFixedPointSetFileName( "" ); };

  /** Set/Get/Remove moving point set filename. */
  itkSetStringMacro( MovingPointSetFileName );
  itkGetConstReferenceMacro( MovingPointSetFileName, std::string );
  void RemoveMovingPointSetFileName( void ) { this->SetMovingPointSetFileName( "" ); };

  /** Set/Get/Remove output directory. */
  itkSetMacro( OutputDirectory, std::string );
  itkGetConstReferenceMacro( OutputDirectory, std::string );
  void RemoveOutputDirectory() { this->SetOutputDirectory( "" ); };

  /** Set/Get/Remove log filename. */
  void SetLogFileName( const std::string logFileName );
  itkGetConstMacro( LogFileName, std::string );
  void RemoveLogFileName( void );

  /** Log to std::cout on/off. */
  itkSetMacro( LogToConsole, bool );
  itkGetConstReferenceMacro( LogToConsole, bool );
  itkBooleanMacro( LogToConsole );

  /** Log to file on/off. */
  itkSetMacro( LogToFile, bool );
  itkGetConstReferenceMacro( LogToFile, bool );
  itkBooleanMacro( LogToFile );

protected:

  ElastixFilter( void );

  virtual void GenerateData( void ) ITK_OVERRIDE;

private: 

  ElastixFilter( const Self & );  // purposely not implemented
  void operator=( const Self & ); // purposely not implemented

  /** SetInputWithUniqueName. */
  void SetInputWithUniqueName( const DataObjectIdentifierType & key, itk::DataObject* input );

  /** IsInputType. */
  bool IsInputType( const DataObjectIdentifierType & inputType, DataObjectIdentifierType inputName );

  /** GetNumberOfInputType */
  unsigned int GetNumberOfInputType( const DataObjectIdentifierType & intputType );

  /** RemoveInputType. */
  void RemoveInputType( const DataObjectIdentifierType & inputName );

  /** Let elastix handle input verification internally */
  virtual void VerifyInputInformation( void ) ITK_OVERRIDE {};

  std::string m_InitialTransformParameterFileName;
  std::string m_FixedPointSetFileName;
  std::string m_MovingPointSetFileName;

  std::string m_OutputDirectory;
  std::string m_LogFileName;

  bool m_LogToConsole;
  bool m_LogToFile;

  unsigned int m_InputUID;

};

} // namespace elx

#ifndef ITK_MANUAL_INSTANTIATION
#include "elxElastixFilter.hxx"
#endif

#endif // elxElastixFilter_h
