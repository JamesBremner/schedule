class cJobType
{
public:
    cJobType()
    {

    }
    cJobType( const std::string& name, int crew )
        : myName( name)
        , myCrew( crew )
    {

    }
    std::string Name()
    {
        return myName;
    }
    int Crew()
    {
        return myCrew;
    }
    void CrewType( const std::vector< std::string >& type )
    {
        myCrewType = type;
    }
    std::vector< std::string >& CrewType()
    {
        return myCrewType;
    }
private:
    std::string myName;
    int myCrew;
    std::vector< std::string > myCrewType;
};

class cJob
{
public:
    cJob( const std::string& name,
         const std::string& type )
        : myType( type )
        , myName( name )
    {

    }
    std::string Type()
    {
        return myType;
    }
    void Type( const std::string& t )
    {
        myType = t;
    }
    std::string Name()
    {
        return myName;
    }
    void Name( const std::string n )
    {
        myName = n;
    }

private:
    std::string myType;
    std::string myName;
};

class cResourceType
{
public:
    cResourceType()
    {

    }
    cResourceType( const std::string& type )
        : myType( type )
    {

    }
    std::string Type()
    {
        return myType;
    }
private:
    std::string myType;
};

class cResource
{
public:
    cResource( const std::string& name,
               const std::string& type )
        : myName( name )
        , myType( type )
        , myfAssign( false )
        , myLastShift( -10 )
    {

    }
    std::string Name()
    {
        return myName;
    }
    void Name( const std::string& n )
    {
        myName = n;
    }
    std::string Type()
    {
        return myType;
    }
    void Type( const std::string& t )
    {
        myType = t;
    }
    std::string Text()
    {
        return myType + " " + myName;
    }

    void Assign( bool f, int shift = -1 );

    bool Assign() const
    {
        return myfAssign;
    }
    int LastShift() const
    {
        return myLastShift;
    }

    /** True if available for a shift
        @param[in] shift for potential assignment to
        @param[in] rotation minimum number of shifts between assignments + 1
    */
    bool IsAvailable( int shift, int rotation ) const;

private:
    std::string myName;
    std::string myType;
    bool myfAssign;
    int myLastShift;
};

class cFleet
{
public:
    cFleet( nana::form& fm );
    void Read();
    void Write();

    void ResourceEditor();
    void JobEditor();
    void ConstraintEditor() {}
    void Add( const cJob& v )
    {
        myJobVector.push_back( v );
    }

    void NewJobType();

    void NewResourceType( nana::form& fm )
    {
        nana::inputbox::text name("Name of type", "");

        nana::inputbox inbox( fm, "New Person Type" );

        if( inbox.show( name ) )
        {
            cResourceType pt;
            if( FindPersonType( name.value(), pt ))
            {
                nana::msgbox msg("Already have this person type");
                msg.show();
            }
            else
            {
                myResourceTypeVector.push_back( cResourceType(
                                                  name.value() ) );
            }
        }
    }
    void NewJob();

    void NewResource();

    void Display();

    bool Schedule( int shifts )
    {
        // clear all previous assignments
        myAssign.clear();

        // loop over shifts
        for( int kshift = 0; kshift < shifts; kshift++ )
        {
            // clear previous shift's assignments
            for( auto& p : myResourceVector )
            {
                p.Assign( false );
            }

            // the assignments for this shift
            std::vector< std::pair< cJob, cResource > > shift_assignments;

            // loop over vehicles
            for( auto& v : myJobVector )
            {
                cJobType vt;
                if( ! FindType( v.Type(), vt ) )
                    throw std::runtime_error("Vehicle type error");

                // loop over people needed by vehicle type
                for( int kp = 0; kp < vt.Crew(); kp++ )
                {
                    bool success = false;

                    // loop over people
                    for( auto& p : myResourceVector )
                    {
                        if( ! p.IsAvailable( kshift, myShiftRotation ) )
                            continue;
                        if( p.Type() != vt.CrewType()[kp] )
                            continue;

                        // assign person to vehicle
                        shift_assignments.push_back( std::pair< cJob, cResource >( v, p ) );
                        p.Assign( true, kshift );
                        success = true;
                        break;
                    }
                    if( ! success )
                    {
                        nana::msgbox msg("Not enough people to crew the fleet");
                        msg.show();
                        myAssign.clear();
                        return false;
                    }
                }
            }
            myAssign.push_back( shift_assignments );

            std::random_shuffle( myResourceVector.begin(), myResourceVector.end() );
        }
        return true;
    }

    void JobTerm( const std::string& term )
    {
        myJobTerm = term;
    }
    std::string JobTerm() const
    {
        return myJobTerm;
    }
    void ResourceTerm( const std::string& term )
    {
        myResourceTerm = term;
    }
    std::string ResourceTerm() const
    {
        return myResourceTerm;
    }
    void Rotation( int r )
    {
        myShiftRotation = r;
    }
    void Test()
    {
//        myResourceTypeVector.push_back( cResourceType("officer"));
//        myResourceTypeVector.push_back( cResourceType("firefighter"));
//        myTypeVector.push_back( cJobType("rescue",2));
//        myTypeVector.back().CrewType({"officer","firefighter"});
//        myJobVector.push_back( cJob("rescue","A"));
//        myResourceVector.push_back( cResource("Alice","officer"));
//        myResourceVector.push_back( cResource("Bob","firefighter"));
    }

private:

    std::string myJobTerm;
    std::string myResourceTerm;
    int myShiftRotation;
    std::vector< cJob > myJobVector;
    std::vector< cJobType > myTypeVector;
    std::vector< cResource > myResourceVector;
    std::vector< cResourceType > myResourceTypeVector;
    std::vector< std::vector< std::pair< cJob, cResource > > > myAssign;
    nana::form& myfm;
    nana::textbox fleet_text;

    bool FindType( const std::string& type_name,
                   cJobType& type );

    bool FindPersonType( const std::string& type_name,
                         cResourceType& type )
    {
        for( auto& t : myResourceTypeVector )
            if( t.Type() == type_name )
            {
                type = t;
                return true;
            }
        return false;
    }
    bool FindPerson( const std::string& type )
    {
        for( auto& p : myResourceVector )
            if( p.Name() == type )
                return true;
        return false;
    }
};

