<?hh

namespace Stick;

use HackPack\HackUnit\Core\Expectation;
use HackPack\HackUnit\Core\TestCase;

class LexerTest extends TestCase
{
    public function testEOF(): void
    {
        $template = "";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[0]->getType())->toEqual(Token::TYPE_EOF);
    }

    public function testReturnsTextBlock(): void
    {
        $template = "Hello World";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[0]->getType())->toEqual(Token::TYPE_TEXT);
        $this->expect($tokens[0]->getValue())->toEqual($template);
    }


    public function testReturnsTextBlockAtStarts(): void
    {
        $startString = "Hello World";
        $template = "$startString{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[0]->getType())->toEqual(Token::TYPE_TEXT);
        $this->expect($tokens[0]->getValue())->toEqual($startString);
    }

    public function testReturnsCodeBlock(): void
    {
        $startString = "Hello World";
        $template = "$startString{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[1]->getType())->toEqual(Token::TYPE_BLOCK_START);
        $this->expect($tokens[1]->getValue())->toEqual("");
    }

    public function testFindsNameTagFor(): void
    {
        $template = "{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[1]->getType())->toEqual(Token::TYPE_NAME);
        $this->expect($tokens[1]->getValue())->toEqual("for");
    }

    public function testFindsNameTagVar(): void
    {
        $template = "{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[2]->getType())->toEqual(Token::TYPE_NAME);
        $this->expect($tokens[2]->getValue())->toEqual("var");
    }

    public function testFindsOperatorTokenIn(): void
    {
        $template = "{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[3]->getType())->toEqual(Token::TYPE_OPERATOR);
        $this->expect($tokens[3]->getValue())->toEqual("in");
    }

    public function testFindsNameTokenArray(): void
    {
        $template = "{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[4]->getType())->toEqual(Token::TYPE_NAME);
        $this->expect($tokens[4]->getValue())->toEqual("array");
    }


    public function testFindsEndBlockToken(): void
    {
        $template = "{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[5]->getType())->toEqual(Token::TYPE_BLOCK_END);
    }


    public function testFindsTextData(): void
    {
      $template = "{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

      $lexer = new Lexer();
      $tokens = $lexer->tokenize($template);
      $this->expect($tokens[6]->getType())->toEqual(Token::TYPE_TEXT);
      $this->expect($tokens[6]->getValue())->toEqual("Hello ");
    }


    public function testFindsVariableStart(): void
    {
        $template = "{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[7]->getType())->toEqual(Token::TYPE_VARIABLE_START);
    }


    public function testFindsNameInVar(): void
    {
        $template = "{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[8]->getType())->toEqual(Token::TYPE_NAME);
        $this->expect($tokens[8]->getValue())->toEqual("var");
    }


    public function testFindsEndVariable(): void
    {
        $template = "{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[9]->getType())->toEqual(Token::TYPE_VARIABLE_END);
    }

    public function testFindsText(): void
    {
        $template = "{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[10]->getType())->toEqual(Token::TYPE_TEXT);
        $this->expect($tokens[10]->getValue())->toEqual(" ");
    }

    public function testReturnsStartBlockAfterText(): void
    {
        $template = "{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[11]->getType())->toEqual(Token::TYPE_BLOCK_START);
    }

    public function testReturnsNameTokenEndFor(): void
    {
        $template = "{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[12]->getType())->toEqual(Token::TYPE_NAME);
        $this->expect($tokens[12]->getValue())->toEqual("endfor");
    }

    public function testReturnsEndBlockAfterText(): void
    {
        $template = "{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[13]->getType())->toEqual(Token::TYPE_BLOCK_END);
    }

    public function testReturnsLastText(): void
    {
        $startString = "Hello World";
        $template = "{% for var in array %}Hello {{ var }} {% endfor %}. Whoop";

        $lexer = new Lexer();
        $tokens = $lexer->tokenize($template);
        $this->expect($tokens[14]->getType())->toEqual(Token::TYPE_TEXT);
        $this->expect($tokens[14]->getValue())->toEqual(". Whoop");
    }

    public function testIgnoresCommentAndReturnsText(): void
    {
          $template = "{# Test comment #}. Whoop";

          $lexer = new Lexer();
          $tokens = $lexer->tokenize($template);
          $this->expect($tokens[0]->getType())->toEqual(Token::TYPE_TEXT);
          $this->expect($tokens[0]->getValue())->toEqual(". Whoop");
    }

    public function testThrowsExceptionWhenCommentNotEnded(): void
    {
        $template = "{# stuff here";

        $lexer = new Lexer();
        $this->expectCallable(() ==> {$lexer->tokenize($template);})->toThrow("Stick\\Exception\\InvalidSyntax");
    }

    public function testThrowsExceptionWhenBlockNotEndedSpace(): void
    {
        $template = "{% dsfjalkds ";

        $lexer = new Lexer();
        $this->expectCallable(() ==> {$lexer->tokenize($template);})->toThrow("Stick\\Exception\\InvalidSyntax");
    }

    public function testThrowsExceptionWhenBlockNotEnded(): void
    {
        $template = "{% dsfjalkds";
        $lexer = new Lexer();
        $this->expectCallable(() ==> {$lexer->tokenize($template);})->toThrow("Stick\\Exception\\InvalidSyntax");
    }

    public function testThrowsExceptionWhenVariableNotEnded(): void
    {
      $template = "{{ dsfjalkds";
      $lexer = new Lexer();
      $this->expectCallable(() ==> {$lexer->tokenize($template);})->toThrow("Stick\\Exception\\InvalidSyntax");
    }
}
