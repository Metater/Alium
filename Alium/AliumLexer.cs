using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace Alium
{
    public class AliumLexer
    {
        private readonly string source;
        private int readPos = 0;
        public bool eof { get; private set; } = false; // make Eof
        private char cc;

        private readonly string[] sourceLines;
        private int currentLine = 0;
        private int currentLineChar = 0;

        private List<char> markerChars = new List<char> { '{', '}', '(', ')', '.', ';', '^', '?', '/' };
        private List<char> castChars = new List<char> { 'i', 'l', 'f', 'd' };

        private int braces = 0;
        private int parenthesis = 0;

        public AliumLexer(string source)
        {
            this.source = source;
            sourceLines = Regex.Split(source, "\r\n|\r|\n");
            if (source.Length < 1) return;
            cc = source[0];
        }

        private void Advance(int amt = 1)
        {
            readPos += amt;
            if (source.Length > readPos)
            {
                cc = source[readPos];
                currentLineChar++;
            }
            else
                eof = true;
        }

        private char Peek(int depth = 1)
        {
            if (PeekIsEof(depth))
                return ' ';
            else return source[readPos + depth];
        }

        private bool PeekIsEof(int depth = 1)
        {
            return readPos + depth > source.Length - 1;
        }

        public string GetErrorMessage(string message)
        {
            return GetErrorMessageLineChar(message, currentLine, currentLineChar);
        }

        private string GetErrorMessageLineChar(string message, int line, int lineChar)
        {
            return $"error: {message}\n\ton line {line + 1} and char index {lineChar}\n\t{line + 1}: \"{sourceLines[line].Insert(lineChar, "<---(ERROR)---")}\"";
        }

        private void SkipWhitespace()
        {
            while (char.IsWhiteSpace(cc) && !cc.Equals('\n') && !cc.Equals('\r') && !eof)
                Advance();
        }

        private Token ReadNumeric()
        {
            string result = "";
            bool foundCast = false;
            char cast = ' ';
            bool foundDecimal = false;
            while ((char.IsDigit(cc) || castChars.Contains(cc) || (cc.Equals('.') && !foundDecimal)) && !eof)
            {
                char c = cc;
                Advance();
                cast = c;
                if (castChars.Contains(c))
                {
                    foundCast = true;
                    break;
                }
                else
                {
                    if (c.Equals('.')) foundDecimal = true;
                    result += c;
                }
            }
            if (!foundCast) return new ErrorToken((GetErrorMessage("literal numeric must have cast")));
            switch (cast)
            {
                case 'i':
                    if (int.TryParse(result, out int parsedInt))
                        return new IntToken(parsedInt);
                    else
                        return new ErrorToken(GetErrorMessage("failed to parse int"));
                case 'l':
                    if (long.TryParse(result, out long parsedLong))
                        return new LongToken(parsedLong);
                    else
                        return new ErrorToken(GetErrorMessage("failed to parse long"));
                case 'f':
                    if (float.TryParse(result, out float parsedFloat))
                        return new FloatToken(parsedFloat);
                    else
                        return new ErrorToken(GetErrorMessage("failed to parse float"));
                case 'd':
                    if (double.TryParse(result, out double parsedDouble))
                        return new DoubleToken(parsedDouble);
                    else
                        return new ErrorToken(GetErrorMessage("failed to parse double"));
                default:
                    return new ErrorToken(GetErrorMessage("failed to parse numeric"));
            }
        }

        private Token ReadIdentifier()
        {
            string result = "";
            while (!char.IsWhiteSpace(cc) && !markerChars.Contains(cc) && !eof)
            {
                result += cc;
                Advance();
            }
            return new IdentifierToken(result);
        }

        private Token ReadString()
        {
            string result = "";
            int line = currentLine;
            int lineChar = currentLineChar;
            Advance();
            while (!cc.Equals('\"') && !eof)
            {
                if (cc.Equals('\n') || cc.Equals('\r'))
                    return new ErrorToken(GetErrorMessage("string literal not opened and closed on one line"));
                result += cc;
                Advance();
            }
            if (eof) return new ErrorToken(GetErrorMessageLineChar("string literal not opened and closed on one line", line, lineChar));
            Advance();
            return new StringToken(result);
        }

        public Token GetNextToken()
        {
            while (!eof)
            {
                if (char.IsWhiteSpace(cc) && !cc.Equals('\n') && !cc.Equals('\r'))
                {
                    SkipWhitespace();
                    return new SpecialToken("whitespace");
                }
                else if (cc.Equals('\n') || cc.Equals('\r'))
                {
                    if (cc.Equals('\r') && Peek().Equals('\n')) Advance();
                    Advance();
                    currentLine++;
                    currentLineChar = 0;
                    return new SpecialToken("newline");
                }
                else if (char.IsDigit(cc))
                    return ReadNumeric();
                else if (markerChars.Contains(cc))
                {
                    char c = cc;
                    Advance();
                    switch (c)
                    {
                        case '{':
                            braces++;
                            break;
                        case '}':
                            braces--;
                            if (braces < 0) return new ErrorToken(GetErrorMessage("too many close braces"));
                            break;
                        case '(':
                            parenthesis++;
                            break;
                        case ')':
                            parenthesis--;
                            if (parenthesis < 0) return new ErrorToken(GetErrorMessage("too many close parenthesis"));
                            break;
                    }
                    return new MarkerToken(c);
                }
                else if (cc.Equals('\"'))
                {
                    return ReadString();
                }
                else
                    return ReadIdentifier();
            }
            if (braces > 0) return new ErrorToken($"error: {braces} unclosed brace(s)");
            if (parenthesis > 0) return new ErrorToken($"error: {parenthesis} unclosed parenthesis");
            return new SpecialToken("eof");
        }
    }
}
